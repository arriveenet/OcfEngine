#include "Scene.h"
#include "base/Game.h"
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

void Scene::render(Renderer* renderer, const glm::mat4& transform)
{
	Scene* pCurrentScene = Game::getInstance()->getCurrentScene();
	glm::mat4 view = pCurrentScene->getDefaultCamera()->getViewMatrix();
	glm::mat4 modelView = view * transform;

	Entity::visit(renderer, modelView);

	renderer->draw();
}

OCF_END
