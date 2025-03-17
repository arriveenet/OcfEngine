#include "FontAtlas.h"
#include "renderer/Texture2D.h"

NS_OCF_BEGIN

int FontAtlas::TextureWidth = 1024;
int FontAtlas::TextureHeight = 1024;

FontAtlas::FontAtlas()
    : m_width(TextureWidth)
    , m_height(TextureHeight)
{
}

FontAtlas::~FontAtlas()
{
    releaseTextures();
}

void FontAtlas::addNewPage()
{
}

void FontAtlas::releaseTextures()
{
    for (auto& texture : m_atlasTextures) {
        texture.second->release();
    }
    m_atlasTextures.clear();
}

void FontAtlas::setTexure(unsigned int slot, Texture2D* texture)
{
    texture->retain();
    m_atlasTextures[slot] = texture;
}

Texture2D* FontAtlas::getTexture(unsigned int slot) const
{
    return m_atlasTextures.at(slot);
}

NS_OCF_END
