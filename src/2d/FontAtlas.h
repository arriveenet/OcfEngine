#pragma once
#include "base/GameObject.h"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

NS_OCF_BEGIN

class Texture2D;
class Rect;
class MaxRectsBinPack;

class FontAtlas : public GameObject {
public:
    static constexpr int DEFAULT_TEXTURE_WIDTH = 512;
    static constexpr int DEFAULT_TEXTURE_HEIGHT = 512;

    FontAtlas();
    FontAtlas(int width, int height);
    virtual ~FontAtlas();

    void createNewPage();

    void addNewPage();

    int getCurrentPage() const { return m_currentPage; }

    bool insert(Rect& outRect, uint8_t* bitmap, int width, int height);

    void updateTexture();

    size_t getPageCount() const { return m_atlasTextures.size(); }

    void releaseTextures();

    void setTexure(unsigned int slot, Texture2D* texture);
    Texture2D* getTexture(unsigned int  slot) const;

private:
    std::unordered_map<unsigned int, Texture2D*> m_atlasTextures;
    int m_width;
    int m_height;
    uint8_t* m_currentPageData;
    int m_currentPageDataSize;
    int m_currentPage;
    std::unique_ptr<MaxRectsBinPack> m_binPack;
    glm::ivec2 m_updateRangePosition;
    glm::ivec2 m_updateRangetSize;

};

NS_OCF_END