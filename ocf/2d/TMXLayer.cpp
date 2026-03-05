#include "TMXLayer.h"
#include "2d/TMXXMLParser.h"
#include "2d/TMXTiledMap.h"
#include "base/Game.h"
#include "renderer/Renderer.h"
#include "renderer/TextureManager.h"

namespace ocf {

TMXLayer* TMXLayer::create(TMXTilesetInfo* tilesetInfo, TMXLayerInfo* layerInfo, TMXMapInfo* mapInfo)
{
    TMXLayer* layer = new TMXLayer();
    if (layer->init(tilesetInfo, layerInfo, mapInfo)) {
        layer->autorelease();
        return layer;
    }
    OCF_SAFE_DELETE(layer);
    return nullptr;
}

TMXLayer::TMXLayer()
    : m_quadsDirty(true)
    , m_layerSize(0, 0)
    , m_pTiles(nullptr)
    , m_pTileset(nullptr)
    , m_pLayerInfo(nullptr)
    , m_pTexture(nullptr)
{
    setName("TMXLayer");
}

TMXLayer::~TMXLayer()
{
    OCF_SAFE_RELEASE(m_pLayerInfo);
}

bool TMXLayer::init(TMXTilesetInfo* tilesetInfo, TMXLayerInfo* layerInfo, TMXMapInfo* /*mapInfo*/)
{
    m_pTileset = tilesetInfo;

    m_pLayerInfo = layerInfo;
    m_layerName = layerInfo->m_name;
    m_layerSize = layerInfo->m_layerSize;
    m_pTiles = layerInfo->m_pTiles;

    m_pTexture = m_pGame->getTextureManager()->addImage("tiledmap/" + m_pTileset->m_imageSource);

    Program* pProgram = ShaderManager::getInstance()->getBuiltinProgram(ProgramType::Basic);
    m_quadCommand.getProgramState().setProgram(pProgram);

    return true;
}

void TMXLayer::draw(Renderer* renderer, const glm::mat4& transform)
{
    updateTotalQuads();

    glm::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
    auto& programState = m_quadCommand.getProgramState();
    programState.setUniform("uMVPMatrix", &projection, sizeof(projection));

    m_quadCommand.init(m_globalZOrder, m_pTexture, m_totalQuads.data(), m_indices.data(), m_totalQuads.size(), transform);
    renderer->addCommand(&m_quadCommand);
}

void TMXLayer::updateTotalQuads()
{
    if (!m_quadsDirty)
        return;

    const glm::vec2 tileSize = m_pTileset->m_tileSize;

    m_totalQuads.resize(size_t(m_layerSize.x * m_layerSize.y));
    m_indices.resize(size_t(m_layerSize.x * m_layerSize.y * 6));

    int quadIndex = 0;
    for (int y = 0; y < m_layerSize.y; y++) {
        for (int x = 0; x < m_layerSize.x; x++) {
            int tileIndex = getTileIndex(x, y);
            int tileGID = m_pTiles[tileIndex];

            if (tileGID == 0)
                continue;

            glm::vec2 nodePosition(x * tileSize.x, y * tileSize.y);

            auto& quad = m_totalQuads[quadIndex];
            float left, right, top, bottom, z;
            z = 0.0f;
            left   = nodePosition.x;
            right  = nodePosition.x + tileSize.x;
            bottom = nodePosition.y;
            top    = nodePosition.y + tileSize.y;

            quad.bottomLeft.position  = {left, bottom, z};
            quad.bottomRight.position = { right, bottom, z };
            quad.topLeft.position     = { left, top, z };
            quad.topRight.position    = { right, top, z };

            std::swap(quad.bottomLeft.position, quad.topLeft.position);
            std::swap(quad.bottomRight.position, quad.topRight.position);

            Rect tileTexture = m_pTileset->getRectForGID(tileGID);
            left   = tileTexture.m_position.x / m_pTexture->getSize().x;
            right  = left + (tileTexture.m_size.x / m_pTexture->getSize().x);
            top = tileTexture.m_position.y / m_pTexture->getSize().y;
            bottom    = top + (tileTexture.m_size.y / m_pTexture->getSize().y);

            quad.bottomLeft.texCoord  = { left, bottom };
            quad.bottomRight.texCoord = { right, bottom };
            quad.topLeft.texCoord     = { left, top };
            quad.topRight.texCoord    = { right, top };

            int i = quadIndex;
            m_indices[(size_t)i * 6 + 0] = (unsigned short)i * 4 + 0;
            m_indices[(size_t)i * 6 + 1] = (unsigned short)i * 4 + 1;
            m_indices[(size_t)i * 6 + 2] = (unsigned short)i * 4 + 2;
            m_indices[(size_t)i * 6 + 3] = (unsigned short)i * 4 + 3;
            m_indices[(size_t)i * 6 + 4] = (unsigned short)i * 4 + 2;
            m_indices[(size_t)i * 6 + 5] = (unsigned short)i * 4 + 1;

            quadIndex++;
        }
    }

    m_quadsDirty = false;
}

} // namespace ocf
