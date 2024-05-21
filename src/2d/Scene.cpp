#include "Scene.h"
#include "2d/Camera.h"
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
	m_pDefaultCamera = new Camera();
	m_pDefaultCamera->init();

	addChild(m_pDefaultCamera);

	return true;
}

void Scene::render(Renderer* renderer, const glm::mat4& transform)
{
	Scene* pCurrentScene = Game::getInstance()->getCurrentScene();
	Camera* pCamera = pCurrentScene->getDefaultCamera();

	Camera::s_pVisitingCamera = pCamera;

	glm::mat4 modelView = pCamera->getViewMatrix();

	m_pGame->pushMatrix(MatrixStack::Projection);
	m_pGame->loadMatrix(MatrixStack::Projection, Camera::s_pVisitingCamera->getProjectionMatrix());

	Node::visit(renderer, modelView, 0);

	renderer->draw();

	m_pGame->popMatrix(MatrixStack::Projection);

	Camera::s_pVisitingCamera = nullptr;
}

OCF_END
