#pragma once
#include <unordered_map>
#include "2d/Font.h"

namespace ocf {

struct FontFreeTypeConfig;

class FontManager {
public:
    static Font* getFontFNT(std::string_view fontFileName);
    static Font* getFontTTF(const FontFreeTypeConfig& config);

    static void release();

private:
    static std::unordered_map<std::string, Font*> m_fontMap;
};

} // namespace ocf
