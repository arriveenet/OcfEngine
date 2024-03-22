#include "Scene.h"

OCF_BEGIN

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
}

void Scene::draw(Renderer* renderer, const glm::mat4& transform)
{
	for (auto& entity : m_entities) {
		entity->draw(renderer, m_transform);
	}
}

OCF_END
