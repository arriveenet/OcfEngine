#pragma once
#include <unordered_map>
#include "base/GameObject.h"

NS_OCF_BEGIN

class Texture2D;

class FontAtlas : public GameObject {
public:
    static int TextureWidth;
    static int TextureHeight;

    FontAtlas();
    virtual ~FontAtlas();

    void addNewPage();

    size_t getPageCount() const { return m_atlasTextures.size(); }

    void releaseTextures();

    void setTexure(unsigned int slot, Texture2D* texture);
    Texture2D* getTexture(unsigned int  slot) const;

protected:
    std::unordered_map<unsigned int, Texture2D*> m_atlasTextures;
    int m_width;
    int m_height;

};

NS_OCF_END