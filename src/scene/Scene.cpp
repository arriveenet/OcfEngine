#include "Scene.h"
#include "renderer/Renderer.h"

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

void Scene::draw(Renderer* renderer, const glm::mat4& transform)
{
	Entity::visit(renderer, transform, this->getID());

	renderer->draw();
}

OCF_END
