#pragma once
#include <string>
#include "2d/Node2D.h"
#include "renderer/QuadCommand.h"

NS_OCF_BEGIN

class TMXLayerInfo;
class TMXTilesetInfo;
class TMXMapInfo;
class Texture2D;

class TMXLayer : public Node2D {
public:
    static TMXLayer* create(TMXTilesetInfo* tilesetInfo, TMXLayerInfo* layerInfo, TMXMapInfo* mapInfo);

    TMXLayer();
    virtual ~TMXLayer();

    bool init(TMXTilesetInfo* tilesetInfo, TMXLayerInfo* layerInfo, TMXMapInfo* mapInfo);
    void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
    void updateTotalQuads();
    int getTileIndex(int x, int y) const { return x + y * m_layerSize.x; }

    bool m_quadsDirty;
    std::string m_layerName;
    glm::ivec2 m_layerSize;
    uint32_t* m_pTiles;
    TMXLayerInfo* m_pLayerInfo;
    TMXTilesetInfo* m_pTileset;
    std::vector<QuadV3fC3fT2f> m_totalQuads;
    std::vector<unsigned short> m_indices;
    QuadCommand m_quadCommand;
    Texture2D* m_pTexture;
};

NS_OCF_END