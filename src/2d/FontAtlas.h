#pragma once
#include <unordered_map>
#include <memory>
#include "base/GameObject.h"

NS_OCF_BEGIN

class Texture2D;
class RectBinPack;

class FontAtlas : public GameObject {
public:
    static constexpr int DEFAULT_TEXTURE_WIDTH = 1024;
    static constexpr int DEFAULT_TEXTURE_HEIGHT = 1024;

    FontAtlas();
    FontAtlas(int width, int height);
    virtual ~FontAtlas();

    void createNewPage();

    void addNewPage();

    bool insertCharactor(uint8_t* bitmap, int width, int height);

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
    std::unique_ptr<RectBinPack> m_binPack;
    int m_updateOffsetX;
    int m_updateOffsetY;
    int m_updateWidth;
    int m_updateHeight;

};

NS_OCF_END