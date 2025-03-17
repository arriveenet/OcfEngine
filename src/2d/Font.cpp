#include "Font.h"
#include "2d/FontAtlas.h"
#include "renderer/Texture2D.h"

NS_OCF_BEGIN

Font::Font()
    : m_pFontAtlas(nullptr)
    , m_lineHeight(0.0f)
{
}

Font::~Font()
{
    if (m_pFontAtlas != nullptr) {
        m_pFontAtlas->release();
    }
}

void Font::addCharacterDefinition(char32_t utf32char, const FontCharacterDefinition& defintition)
{
    m_characterDefinition[utf32char] = defintition;
}

bool Font::getCharacterDefinition(char32_t utf32char, FontCharacterDefinition& defintition)
{
    auto iter = m_characterDefinition.find(utf32char);
    if (iter != m_characterDefinition.end()) {
        defintition = (*iter).second;
        return true;
    }
    else {
        return false;
    }
}

void Font::setLineHeight(float lineHeight)
{
    m_lineHeight = lineHeight;
}

std::string_view Font::getFontName() const
{
    return m_fontName;
}

NS_OCF_END
