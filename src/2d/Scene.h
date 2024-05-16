#pragma once
#include <vector>
#include "2d/Entity.h"

OCF_BEGIN

class Camera;

class Scene : public Entity {
public:
	Scene();
	virtual ~Scene();

	virtual bool init();

	void render(Renderer* renderer, const glm::mat4& transform);

	virtual Camera* getDefaultCamera() { return m_pDefaultCamera; }

protected:
	Camera* m_pDefaultCamera;
};

OCF_END
