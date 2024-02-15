#pragma once
#include <vector>
#include "../Camera.h"

class Scene : public Entity {
public:
	Scene();
	virtual ~Scene();

	virtual bool init();

	virtual void update() = 0;
	virtual void update(float deltaTime);
	virtual void draw() = 0;

	virtual void onKeyEnvet(int key, int scancode, int action, int mods) {};

	virtual Camera* getDefaultCamera() { return &m_defaultCamera; }

protected:
	Camera m_defaultCamera;
};

