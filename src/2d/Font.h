#pragma once
#include <string>
#include <unordered_map>
#include "base/types.h"

NS_OCF_BEGIN

class Texture2D;

struct FontCharacterDefinition {
    float x;
    float y;
    float width;
    float height;
    float xoffset;
    float yoffset;
    float xadvance;
};

class Font : public GameObject {
public:
    Font();
    virtual ~Font();

    void addCharacterDefinition(char32_t utf32char, const FontCharacterDefinition& defintition);
    bool getCharacterDefinition(char32_t utf32char, FontCharacterDefinition& defintition);

    float getLineHeight() const { return m_lineHeight; }
    void setLineHeight(float lineHeight);

    std::string_view getFontName() const;
    Texture2D* getTexture();
protected:
    std::string m_fontName;
    Texture2D* m_pTexture;
    float m_lineHeight;
    std::unordered_map<char32_t, FontCharacterDefinition> m_characterDefinition;

    friend class Label;
};

NS_OCF_END
