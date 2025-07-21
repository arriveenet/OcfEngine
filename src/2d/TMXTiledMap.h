#pragma once
#include <string>
#include "2d/Node2D.h"

NS_OCF_BEGIN

class TMXMapInfo;

class TMXTiledMap : public Node2D {
public:
    static TMXTiledMap* create(const std::string& tmxFile);

    TMXTiledMap();
    virtual ~TMXTiledMap();

    bool initWithTMXFile(const std::string& tmxFile);

protected:
    void buildWithMapInfo(TMXMapInfo* mapInfo);

    glm::ivec2 m_mapSize;
    glm::ivec2 m_tileSize;
    std::string m_TMXFilenName;
    TMXMapInfo* m_pMapInfo;
};

NS_OCF_END