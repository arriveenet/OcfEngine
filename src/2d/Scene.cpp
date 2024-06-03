#include "Scene.h"
#include "2d/Camera.h"
#include "base/Game.h"
#include "renderer/Renderer.h"

OCF_BEGIN

Scene::Scene()
{
	m_ignoreAnchorPointForPosition = true;
	setAnchorPoint(glm::vec2(0.5f, 0.5f));

	Camera::s_pVisitingCamera = nullptr;
}

Scene::~Scene()
{
}

bool Scene::init()
{
	m_pDefaultCamera = new Camera();
	m_pDefaultCamera->init();

	addChild(m_pDefaultCamera);

	auto size = m_pGame->getVisibleSize();
	setSize(size);

	return true;
}

void Scene::render(Renderer* renderer, const glm::mat4& eyeProjection)
{
	Scene* pCurrentScene = Game::getInstance()->getCurrentScene();
	Camera* pCamera = pCurrentScene->getDefaultCamera();

	Camera::s_pVisitingCamera = pCamera;

	const auto& transform = getNodeToParentTransform();

	m_pGame->pushMatrix(MatrixStack::Projection);
	m_pGame->loadMatrix(MatrixStack::Projection, Camera::s_pVisitingCamera->getViewProjectionMatrix());

	Node::visit(renderer, transform, 0);

	renderer->draw();

	m_pGame->popMatrix(MatrixStack::Projection);

	Camera::s_pVisitingCamera = nullptr;
}

OCF_END
