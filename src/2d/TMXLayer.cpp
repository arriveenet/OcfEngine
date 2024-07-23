#include "TMXLayer.h"
#include "2d/TMXXMLParser.h"
#include "2d/TMXTiledMap.h"

NS_OCF_BEGIN

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
	, m_pLayerInfo(nullptr)
{
}

TMXLayer::~TMXLayer()
{
	OCF_SAFE_RELEASE(m_pLayerInfo);
}

bool TMXLayer::init(TMXTilesetInfo* tilesetInfo, TMXLayerInfo* layerInfo, TMXMapInfo* mapInfo)
{
	m_pTileset = tilesetInfo;

	m_pLayerInfo = layerInfo;
	m_name = layerInfo->m_name;
	m_layerSize = layerInfo->m_layerSize;
	m_pTiles = layerInfo->m_pTiles;

	return true;
}

void TMXLayer::draw(Renderer* renderer, const glm::mat4& transform)
{
	updateTotalQuads();
}

void TMXLayer::updateTotalQuads()
{
	if (!m_quadsDirty)
		return;

	m_totalQuads.resize(size_t(m_layerSize.x * m_layerSize.y));

	int quadIndex = 0;
	for (int y = 0; y < m_layerSize.y; y++) {
		for (int x = 0; x < m_layerSize.x; x++) {
			int tileIndex = getTileIndex(x, y);
			int tileGID = m_pTiles[tileIndex];

			if (tileGID == 0)
				continue;

			auto& quad = m_totalQuads[quadIndex];

			Rect tileTexture = m_pTileset->getRectForGID(tileGID);

			quadIndex++;
		}
	}

	m_quadsDirty = false;
}

NS_OCF_END
