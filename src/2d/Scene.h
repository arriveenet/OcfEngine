#pragma once
#include <vector>
#include "2d/Node.h"

NS_OCF_BEGIN

class Camera;

class Scene : public Node {
public:
	Scene();
	virtual ~Scene();

	virtual bool init();

	void render(Renderer* renderer, const glm::mat4& transform);

	virtual Camera* getDefaultCamera() { return m_pDefaultCamera; }

protected:
	Camera* m_pDefaultCamera;
};

NS_OCF_END
