#pragma once
#include <string>
#include <unordered_map>
#include "base/Types.h"

namespace ocf {

class Texture2D;
class FontAtlas;

struct FontCharacterDefinition {
    float x;
    float y;
    float width;
    float height;
    float xoffset;
    float yoffset;
    float xadvance;
    int page;
};

class Font : public GameObject {
public:
    Font();
    virtual ~Font();

    void addCharacterDefinition(char32_t utf32char, const FontCharacterDefinition& defintition);
    bool getCharacterDefinition(char32_t utf32char, FontCharacterDefinition& defintition);

    float getLineHeight() const { return m_lineHeight; }
    void setLineHeight(float lineHeight);

    virtual FontAtlas* createFontAtlas() = 0;

    std::string_view getFontName() const;

    FontAtlas* getFontAtlas() const { return m_pFontAtlas; }

protected:
    std::string m_fontName;
    FontAtlas* m_pFontAtlas;
    float m_lineHeight;
    std::unordered_map<char32_t, FontCharacterDefinition> m_characterDefinition;

    friend class Label;
    friend class FontManager;
};

} // namespace ocf
