#include "FontFreeType.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include FT_BBOX_H
#include FT_FONT_FORMATS_H

NS_OCF_BEGIN

FT_Library FontFreeType::s_ftLibrary = nullptr;
bool FontFreeType::s_ftInitialized = false;

FontFreeType* FontFreeType::create(const std::string& fontFileName, int fontSize)
{
    return nullptr;
}

FontFreeType::FontFreeType()
    : m_fontSize(0)
{
}

FontFreeType::~FontFreeType()
{
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

void FontFreeType::releaseFreeType()
{
    if (s_ftInitialized) {
        FT_Done_FreeType(s_ftLibrary);
        s_ftLibrary = nullptr;
        s_ftInitialized = false;
    }
}

FT_Library FontFreeType::getFTLibrary()
{
    initFreeType();
    return s_ftLibrary;
}

NS_OCF_END
