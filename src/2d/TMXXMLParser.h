#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <tinyxml2.h>
#include "base/GameObject.h"
#include "base/Rect.h"

NS_OCF_BEGIN

enum class TMXLayerAttribute {
    None   = 1 << 0,
    Base64 = 1 << 1,
    Gzip   = 1 << 2,
    Zlib   = 1 << 3,
    CSV    = 1 << 4,
};

enum class TMXTileFlag : uint32_t {
    Horizontal = 0x80000000,
    Vertical   = 0x40000000,
    Diagonal   = 0x20000000,
    FipedAll   = (Horizontal | Vertical | Diagonal),
    FippedMask = ~(FipedAll)
};

class TMXLayerInfo : public GameObject {
public:
    TMXLayerInfo();
    virtual ~TMXLayerInfo();

    std::string m_name;
    glm::ivec2 m_layerSize;
    uint32_t* m_pTiles;
};

class TMXTilesetInfo : public GameObject {
public:
    TMXTilesetInfo();
    virtual ~TMXTilesetInfo();

    Rect getRectForGID(uint32_t gid);

public:
    std::string m_name;
    int m_firstGid;
    glm::vec2 m_tileSize;
    std::string m_imageSource;
    glm::vec2 m_imageSize;
};

class TMXMapInfo : public GameObject {
public:
    static TMXMapInfo* create(const std::string& tmxFile);

    TMXMapInfo();
    virtual ~TMXMapInfo();

    bool initWithTMXFile(const std::string& tmxFile);

    const glm::ivec2& getMapSize() const { return m_mapSize; }
    void setMapSize(const glm::ivec2& mapSize) { m_mapSize = mapSize; }

    const glm::ivec2& getTileSize() const { return m_tileSize; }
    void setTileSize(const glm::ivec2& tileSize) { m_tileSize = tileSize; }

    const std::vector<TMXTilesetInfo*>& getTilesets() const { return m_tilesets; }
    std::vector<TMXTilesetInfo*>& getTilesets() { return m_tilesets; }
    TMXTilesetInfo* getTileset() const { return m_tilesetInfo; }

    const std::vector<TMXLayerInfo*>& getLayers() const { return m_layers; }
    std::vector<TMXLayerInfo*>& getLayers() { return m_layers; }

protected:
    bool parseXMLFile(const std::string& xmlFile);
    void parseLayer(tinyxml2::XMLElement* element);

    glm::ivec2 m_mapSize;
    glm::ivec2 m_tileSize;
    TMXTilesetInfo* m_tilesetInfo;
    TMXLayerInfo m_tileLayerInfo;
    std::vector<TMXTilesetInfo*> m_tilesets;
    std::vector<TMXLayerInfo*> m_layers;
};

NS_OCF_END
