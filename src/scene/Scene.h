#pragma once
#include "../Camera.h"
#include <vector>

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual bool init();

	virtual void update() = 0;
	virtual void update(float deltaTime);
	virtual void draw() = 0;

	virtual void onKeyEnvet(int key, int scancode, int action, int mods) {};

	void addEntity(Entity* pEntity);

	Camera m_defaultCamera;

private:
	std::vector<Entity*> m_entities;
};

