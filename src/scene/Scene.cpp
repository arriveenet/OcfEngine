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
}