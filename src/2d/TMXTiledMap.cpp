#include "TMXTiledMap.h"
#include "2d/TMXXMLParser.h"
#include "2d/TMXLayer.h"

NS_OCF_BEGIN

TMXTiledMap* TMXTiledMap::create(const std::string& tmxFile)
{
    TMXTiledMap* tiledMap = new TMXTiledMap();
    if (tiledMap->initWithTMXFile(tmxFile)) {
        tiledMap->autorelease();
        return tiledMap;
    }
    OCF_SAFE_DELETE(tiledMap);
    return nullptr;
}

TMXTiledMap::TMXTiledMap()
    : m_mapSize(0, 0)
    , m_tileSize(0, 0)
    , m_pMapInfo(nullptr)
{
}

TMXTiledMap::~TMXTiledMap()
{
    OCF_SAFE_RELEASE(m_pMapInfo);
}

bool TMXTiledMap::initWithTMXFile(const std::string& tmxFile)
{
    TMXMapInfo* mapInfo = TMXMapInfo::create(tmxFile);
    if (mapInfo == nullptr) {
        return false;
    }

    buildWithMapInfo(mapInfo);

    m_TMXFilenName = tmxFile;

    return true;
}

void TMXTiledMap::buildWithMapInfo(TMXMapInfo* mapInfo)
{
    m_mapSize = mapInfo->getMapSize();
    m_tileSize = mapInfo->getTileSize();
    m_pMapInfo = mapInfo;
    //OCF_SAFE_RETAIN(m_pMapInfo);

    auto& layers = mapInfo->getLayers();
    for (const auto& layerInfo : layers) {
        TMXLayer* layer = TMXLayer::create(mapInfo->getTileset(), layerInfo, mapInfo);
        if (layer == nullptr) {
            continue;
        }

        addChild(layer);
    }
}

NS_OCF_END
