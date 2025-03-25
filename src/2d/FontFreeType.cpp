#include "FontFreeType.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include FT_BBOX_H
#include FT_FONT_FORMATS_H

#include "2d/FontAtlas.h"
#include "base/FileUtils.h"
#include "base/StringUtils.h"

NS_OCF_BEGIN

using namespace std::string_view_literals;

static constexpr std::string_view GLYPH_ASCII =
    "\"!#$%&'()*+,-./"
    "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
    "¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþ "sv;

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
    , m_glyphCollection(GlyphCollection::Dynamic)
{
}

FontFreeType::~FontFreeType()
{
}

FontAtlas* FontFreeType::createFontAtlas()
{
    auto fontAtlas = new FontAtlas();
    fontAtlas->createNewPage();

    if (m_glyphCollection != GlyphCollection::Dynamic) {
        std::u32string utf32Text = StringUtils::convertUtf8ToUtf32(getGlyphCollection());
        prepareLetterDefinitions(utf32Text);
    }

    return fontAtlas;
}

bool FontFreeType::prepareLetterDefinitions(const std::u32string& utf32Text)
{
    return false;
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

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    return true;
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
