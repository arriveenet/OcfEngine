#pragma once
#include <unordered_map>
#include "2d/Font.h"

NS_OCF_BEGIN

class FontManager {
public:
    static Font* getFontFNT(std::string_view fontFileName);

    static void release();

private:
    static std::unordered_map<std::string, Font*> m_fontMap;
};

NS_OCF_END