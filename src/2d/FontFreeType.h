#pragma once
#include "2d/Font.h"
#include "2d/IFontEngine.h"
#include <unordered_set>

NS_OCF_BEGIN

struct FontFreeTypeConfig {
    std::string fontPath;
    int fontSize = 0;
    GlyphCollection glyphs = GlyphCollection::Ascii;
};

class FontFreeType : public Font {
public:
    static FontFreeType* create(const std::string& fontPath, int fontSize, GlyphCollection glyphs);

    static void terminateFreeType();

    FontFreeType();
    virtual ~FontFreeType();

    FontAtlas* createFontAtlas() override;

    bool prepareLetterDefinitions(const std::u32string& utf32Text);

private:
    static FT_Library s_ftLibrary;
    static bool s_ftInitialized;
    
    static bool initFreeType();
    static FT_Library getFTLibrary();

    bool initFont(const std::string_view fontPath, int fontSize);

    void findNewCharacters(const std::u32string& utf32Text,
                           std::unordered_set<char32_t>& charset);

    void setGlyphCollection(GlyphCollection glyphs) { m_glyphCollection = glyphs; }
    std::string_view getGlyphCollection() const;

private:
    int m_fontSize;
    FT_Face m_fontFace;
    GlyphCollection m_glyphCollection;
};

NS_OCF_END