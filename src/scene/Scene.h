#pragma once
#include <vector>
#include "2d/Camera.h"

OCF_BEGIN

class Scene : public Entity {
public:
	Scene();
	virtual ~Scene();

	virtual bool init();

	void update(float deltaTime) override;
	virtual void draw() = 0;

	virtual Camera* getDefaultCamera() { return &m_defaultCamera; }

protected:
	Camera m_defaultCamera;
};

OCF_END
