#pragma once
#include "2d/Font.h"

NS_OCF_BEGIN

class FontFreeType : public Font {
public:
    static FontFreeType* create(const std::string& fontFileName, int fontSize);

    FontFreeType();
    virtual ~FontFreeType();

private:
    static FT_Library s_ftLibrary;
    static bool s_ftInitialized;
    
    static bool initFreeType();
    static void releaseFreeType();
    static FT_Library getFTLibrary();

private:
    std::string m_fontName;
    int m_fontSize;
};

NS_OCF_END