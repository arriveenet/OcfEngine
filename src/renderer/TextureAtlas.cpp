#include "TextureAtlas.h"
#include "base/Game.h"

NS_OCF_BEGIN

TextureAtlas* TextureAtlas::create(const std::string& filename, size_t capacity)
{
    TextureAtlas* textureAtlas = new TextureAtlas();
    if (textureAtlas) {
        if (textureAtlas->initWithFile(filename, capacity)) {
            return textureAtlas;
        }
        else {
            delete textureAtlas;
            textureAtlas = nullptr;
        }
    }

    return nullptr;
}

TextureAtlas::TextureAtlas()
{
}

TextureAtlas::~TextureAtlas()
{
    delete m_pQuads;
    m_pQuads = nullptr;

    delete m_pIndices;
    m_pIndices = nullptr;
}

bool TextureAtlas::initWithFile(const std::string& filename, size_t capacity)
{
    Texture2D* texture = Game::getInstance()->getTextureManager()->addImage(filename);

    if (texture) {
        return initWidthTexture(texture, capacity);
    }

    return false;
}

bool TextureAtlas::initWidthTexture(Texture2D* pTexture, size_t capacity)
{
    m_capacity = capacity;
    m_totalQuads = 0;

    this->m_pTexture = pTexture;

    m_pQuads = new QuadV3fT2f[m_capacity];
    m_pIndices = new unsigned short[m_capacity * 6];

    memset(m_pQuads, 0, m_capacity * sizeof(QuadV3fT2f));
    memset(m_pIndices, 0, m_capacity * 6 * sizeof(unsigned short));

    this->setupIndices();

    return true;
}

void TextureAtlas::insertQuad(QuadV3fT2f* pQuad, size_t index)
{
    assert(index >= 0 && index < m_capacity);

    m_totalQuads++;
    assert(m_totalQuads <= m_capacity);

    size_t remaining = (m_totalQuads - 1) - index;

    if (remaining > 0) {
        memmove(&m_pQuads[index + 1], &m_pQuads[index], sizeof(m_pQuads[0]) * remaining);
    }

    m_pQuads[index] = *pQuad;
}

void TextureAtlas::removeQuad(size_t index)
{
    assert(index >= 0 && index < m_totalQuads);

    size_t remaining = (m_totalQuads - 1) - index;

    if (remaining) {
        memmove(&m_pQuads[index], &m_pQuads[index + 1], sizeof(m_pQuads[0]) * remaining);
    }

    m_totalQuads--;
}

size_t TextureAtlas::getTotalQuads() const
{
    return m_totalQuads;
}

size_t TextureAtlas::getCapacity() const
{
    return m_capacity;
}

Texture2D* TextureAtlas::getTexture() const
{
    return m_pTexture;
}

QuadV3fT2f* TextureAtlas::getQuads() const
{
    return m_pQuads;
}

void TextureAtlas::setupIndices()
{
    if (m_capacity == 0) return;

    for (int i = 0; i < m_capacity; i++) {
        m_pIndices[i * 6 + 0] = i * 4 + 0;
        m_pIndices[i * 6 + 1] = i * 4 + 1;
        m_pIndices[i * 6 + 2] = i * 4 + 2;

        m_pIndices[i * 6 + 3] = i * 4 + 3;
        m_pIndices[i * 6 + 4] = i * 4 + 2;
        m_pIndices[i * 6 + 5] = i * 4 + 1;
    }
}

NS_OCF_END
