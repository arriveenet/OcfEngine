#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::init()
{
	m_defaultCamera.init();

	return true;
}

void Scene::update(float deltaTime)
{
	for (auto& entity : m_entities) {
		entity->update(deltaTime);
	}
}

void Scene::addEntity(Entity* pEntity)
{
	m_entities.push_back(pEntity);
}
