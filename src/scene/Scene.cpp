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
	Camera* pCamera = pCurrentScene->getDefaultCamera();

	glm::mat4 modelView = pCamera->getViewMatrix();

	m_pGame->pushMatrix(MatrixStack::Projection);
	m_pGame->loadMatrix(MatrixStack::Projection, pCamera->getProjectionMatrix());

	Entity::visit(renderer, modelView, FLAGS_TRANSFORM_DIRTY);

	renderer->draw();

	m_pGame->popMatrix(MatrixStack::Projection);
}

OCF_END
