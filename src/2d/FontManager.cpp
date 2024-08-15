#include "FontManager.h"
#include "2d/FontFNT.h"

NS_OCF_BEGIN

std::unordered_map<std::string, Font*> FontManager::m_fontMap;

Font* FontManager::getFontFNT(std::string_view fontFileName)
{
    std::string fileName = fontFileName.data();

    auto iter = m_fontMap.find(fileName);
    if (iter == m_fontMap.end()) {
        FontFNT* font = FontFNT::create(fontFileName);
        if (font != nullptr) {
            m_fontMap.emplace(fileName, font);
            return font;
        }
    }
    else {
        return iter->second;
    }

    return nullptr;
}

void FontManager::release()
{
    for (auto& font : m_fontMap) {
        font.second->release();
    }

    m_fontMap.clear();
}

NS_OCF_END
