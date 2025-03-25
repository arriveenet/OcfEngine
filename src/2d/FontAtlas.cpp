#include "FontAtlas.h"
#include "base/MaxRectsBinPack.h"
#include "renderer/Texture2D.h"

NS_OCF_BEGIN

FontAtlas::FontAtlas()
    : FontAtlas(DEFAULT_TEXTURE_WIDTH, DEFAULT_TEXTURE_HEIGHT)
{
}

FontAtlas::FontAtlas(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_currentPageData(nullptr)
    , m_currentPageDataSize(0)
    , m_currentPage(-1)
    , m_updateOffsetX(0)
    , m_updateOffsetY(0)
    , m_updateWidth(0)
    , m_updateHeight(0)
{
    m_binPack = std::make_unique<MaxRectsBinPack>();
}

FontAtlas::~FontAtlas()
{
    OCF_SAFE_DELETE_ARRAY(m_currentPageData);
    releaseTextures();
}

void FontAtlas::createNewPage()
{
    m_currentPageDataSize = m_width * m_height;
    m_currentPageData = new uint8_t[m_currentPageDataSize];

    addNewPage();
}

void FontAtlas::addNewPage()
{
    m_binPack->init(static_cast<float>(m_width), static_cast<float>(m_height));

    memset(m_currentPageData, 0, m_currentPageDataSize);

    Texture2D* texture = new Texture2D();
    texture->initWithData(m_currentPageData, m_currentPageDataSize,
                          m_width, m_height, PixelFormat::GRAY);

    setTexure(++m_currentPage, texture);
}

bool FontAtlas::insertCharactor(uint8_t* bitmap, int width, int height)
{
    Rect outRect = m_binPack->insert(static_cast<float>(width), static_cast<float>(height));
    if (outRect.m_size.x == 0 || outRect.m_size.y == 0) {
        return false;
    }

    // ビットマップをコピー
    for (int y = 0; y < height; y++) {
        const auto destIndex = m_height * y + static_cast<int>(outRect.m_position.x);
        const auto srcIndex = width * y;
        memcpy(m_currentPageData + destIndex, &bitmap[srcIndex], width);
    }

    // 更新する範囲を計算
    m_updateOffsetX = std::min(m_updateOffsetX, static_cast<int>(outRect.m_position.x));
    m_updateOffsetY = std::min(m_updateOffsetY, static_cast<int>(outRect.m_position.y));
    m_updateWidth = std::max(m_updateWidth, static_cast<int>(outRect.getMaxX()));
    m_updateHeight = std::max(m_updateHeight, static_cast<int>(outRect.getMaxY()));

    return true;
}

void FontAtlas::updateTexture()
{
    m_updateOffsetX = 0;
    m_updateOffsetY = 0;
    m_updateWidth = 0;
    m_updateHeight = 0;
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
