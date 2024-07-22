#include "TMXLayer.h"
#include "2d/TMXXMLParser.h"

NS_OCF_BEGIN

TMXLayer* TMXLayer::create(TMXLayerInfo* layerInfo)
{
	TMXLayer* layer = new TMXLayer();
	if (layer->init(layerInfo)) {
		layer->autorelease();
		return layer;
	}
	OCF_SAFE_DELETE(layer);
	return nullptr;
}

TMXLayer::TMXLayer()
	: m_pLayerInfo(nullptr)
{
}

TMXLayer::~TMXLayer()
{
	OCF_SAFE_RELEASE(m_pLayerInfo);
}

bool TMXLayer::init(TMXLayerInfo* layerInfo)
{
	m_pLayerInfo = layerInfo;
	return true;
}

void TMXLayer::draw(Renderer* renderer, const glm::mat4& transform)
{
}

NS_OCF_END
