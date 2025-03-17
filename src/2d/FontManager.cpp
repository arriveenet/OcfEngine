#include "FontManager.h"
#include "2d/FontAtlas.h"
#include "2d/FontFNT.h"
#include "2d/FontFreeType.h"

NS_OCF_BEGIN

std::unordered_map<std::string, Font*> FontManager::m_fontMap;

Font* FontManager::getFontFNT(std::string_view fontFileName)
{
    std::string fileName = fontFileName.data();

    auto iter = m_fontMap.find(fileName);
    if (iter == m_fontMap.end()) {
        FontFNT* font = FontFNT::create(fontFileName);
        if (font != nullptr) {
            const auto fontAtlas = font->createFontAtlas();
            if (fontAtlas != nullptr) {
                font->m_pFontAtlas = fontAtlas;
                return m_fontMap.emplace(std::move(fileName), font).first->second;
            }
        }
    }
    else {
        return iter->second;
    }

    return nullptr;
}

Font* FontManager::getFontTTF(const FontFreeTypeConfig& config)
{
    std::string fileName = config.fontPath;

    auto iter = m_fontMap.find(fileName);
    if (iter == m_fontMap.end()) {
        FontFreeType* font = FontFreeType::create(fileName,
                                                  config.fontSize,
                                                  config.glyphs);
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
