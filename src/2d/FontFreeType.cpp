#include "FontFreeType.h"

#include "2d/FontAtlas.h"
#include "base/FileUtils.h"
#include "base/Rect.h"
#include "base/StringUtils.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include FT_BBOX_H
#include FT_FONT_FORMATS_H

#include <iterator>

NS_OCF_BEGIN

using namespace std::string_view_literals;

static constexpr std::string_view GLYPH_ASCII =
    "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "[\\] ^ _`abcdefghijklmnopqrstuvwxyz{|}~ "sv;

FT_Library FontFreeType::s_ftLibrary = nullptr;
bool FontFreeType::s_ftInitialized = false;

FontFreeType* FontFreeType::create(const std::string& fontPath, int fontSize, GlyphCollection glyphs)
{
    FontFreeType* pFont = new FontFreeType();

    if (pFont->initFont(fontPath, fontSize)) {
        pFont->setGlyphCollection(glyphs);
        pFont->autorelease();
        return pFont;
    }
    OCF_SAFE_DELETE(pFont);

    return nullptr;
}

void FontFreeType::terminateFreeType()
{
    if (s_ftInitialized) {
        FT_Done_FreeType(s_ftLibrary);
        s_ftLibrary = nullptr;
        s_ftInitialized = false;
    }
}

FontFreeType::FontFreeType()
    : m_fontSize(0)
    , m_ascender(0)
    , m_descender(0)
    , m_fontFace(nullptr)
    , m_glyphCollection(GlyphCollection::Dynamic)
{
}

FontFreeType::~FontFreeType()
{
    if (s_ftInitialized) {
        FT_Done_Face(m_fontFace);
    }
}

FontAtlas* FontFreeType::createFontAtlas()
{
    auto fontAtlas = new FontAtlas();
    fontAtlas->createNewPage();
    m_pFontAtlas = fontAtlas;

    if (m_glyphCollection != GlyphCollection::Dynamic) {
        std::u32string utf32Text = StringUtils::convertUtf8ToUtf32(getGlyphCollection());
        prepareLetterDefinitions(utf32Text);
    }

    return fontAtlas;
}

bool FontFreeType::prepareLetterDefinitions(const std::u32string& utf32Text)
{
    OCF_ASSERT(m_pFontAtlas != nullptr);
    OCF_ASSERT(m_fontFace != nullptr);

    std::unordered_set<char32_t> charCodeSet;
    findNewCharacters(utf32Text, charCodeSet);
    if (charCodeSet.empty()) {
        return false;
    }

    for (auto&& charCode : charCodeSet) {
        auto glyphIndex = FT_Get_Char_Index(m_fontFace, static_cast<FT_ULong>(charCode));
        if (glyphIndex == 0) {
            continue;
        }

        if (FT_Load_Glyph(m_fontFace, glyphIndex, FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT) != 0) {
            continue;
        }

        uint8_t* bitmap = m_fontFace->glyph->bitmap.buffer;
        int width = m_fontFace->glyph->bitmap.width;
        int height = m_fontFace->glyph->bitmap.rows;

        Rect tempRect;
        m_pFontAtlas->insert(tempRect, bitmap, width, height);

        FontCharacterDefinition definition = {};
        definition.x = tempRect.m_position.x;
        definition.y = tempRect.m_position.y;
        definition.width = tempRect.m_size.x;
        definition.height = tempRect.m_size.y;
        definition.xoffset = static_cast<float>(m_fontFace->glyph->bitmap_left);
        definition.yoffset = static_cast<float>(m_lineHeight + m_descender - m_fontFace->glyph->bitmap_top);
        definition.xadvance = static_cast<float>(m_fontFace->glyph->advance.x >> 6);
        definition.page = m_pFontAtlas->getCurrentPage();

        addCharacterDefinition(charCode, definition);
    }

    m_pFontAtlas->updateTexture();

    return true;
}

bool FontFreeType::initFreeType()
{
    if (!s_ftInitialized) {
        if (FT_Init_FreeType(&s_ftLibrary) != 0) {
            return false;
        }

        s_ftInitialized = true;
    }

    return s_ftInitialized;
}

FT_Library FontFreeType::getFTLibrary()
{
    initFreeType();
    return s_ftLibrary;
}

bool FontFreeType::initFont(const std::string_view fontPath, int fontSize)
{
    std::string path = FileUtils::getInstance()->fullPathForFilename(fontPath.data());

    if (path.empty()) {
        return false;
    }

    FT_Face face;
    if (FT_New_Face(getFTLibrary(), path.c_str(), 0, &face) != 0) {
        return false;
    }

    m_fontFace = face;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    m_ascender = static_cast<int>(face->size->metrics.ascender >> 6);
    m_descender = static_cast<int>(face->size->metrics.descender >> 6);
    m_lineHeight = static_cast<float>(face->size->metrics.height >> 6);

    return true;
}

void FontFreeType::findNewCharacters(const std::u32string& utf32Text,
                                     std::unordered_set<char32_t>& charset)
{
    if (m_characterDefinition.empty()) {
        std::copy(utf32Text.begin(), utf32Text.end(),
                  std::inserter(charset, charset.end()));
    }
    else {
        for (auto&& charCode : utf32Text) {
            if (m_characterDefinition.find(charCode) == m_characterDefinition.end()) {
                charset.insert(charCode);
            }
        }
    }
}

std::string_view FontFreeType::getGlyphCollection() const
{
    std::string_view collection;

    switch (m_glyphCollection) {
    case GlyphCollection::Dynamic:
        break;
    case GlyphCollection::Ascii:
        collection = GLYPH_ASCII;
        break;
    case GlyphCollection::Custom:
        break;
    default:
        break;
    }

    return collection;
}

NS_OCF_END
