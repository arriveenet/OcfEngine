#pragma once
#include "2d/Node.h"

NS_OCF_BEGIN

class TMXLayerInfo;

class TMXLayer : public Node {
public:
	static TMXLayer* create(TMXLayerInfo* layerInfo);

	TMXLayer();
	virtual ~TMXLayer();

	bool init(TMXLayerInfo* layerInfo);
	void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
	TMXLayerInfo* m_pLayerInfo;
	std::vector<QuadV3fC3fT2f> m_totalQuads;
};

NS_OCF_END