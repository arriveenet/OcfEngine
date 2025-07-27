#include "Scene.h"
#include "2d/Camera2D.h"
#include "base/Game.h"
#include "base/Viewport.h"
#include "renderer/Renderer.h"

NS_OCF_BEGIN

Scene::Scene()
    : m_pDefaultCamera(nullptr)
{
    Camera2D::s_pVisitingCamera = nullptr;
    m_pGame = Game::getInstance();
    m_root = new Viewport();
    m_pDefaultCamera = new Camera2D();
    m_pDefaultCamera->init();
    m_root->addChild(m_pDefaultCamera);

    auto size = m_pGame->getVisibleSize();
    m_root->setSize(size);


}

Scene::~Scene()
{
    OCF_SAFE_DELETE(m_root);
}

bool Scene::init()
{
    OCF_ASSERT(m_root);
    m_root->setScene(this);
    return true;
}

void Scene::update(float deltaTime)
{
    OCFASSERT(m_root, "Scene root is not initialized.");
    m_root->update(deltaTime);
}

void Scene::render(Renderer* renderer, const glm::mat4& eyeProjection )
{
    OCFASSERT(m_root, "Scene root is not initialized.");

    m_pGame->pushMatrix(MatrixStack::Projection);
    m_pGame->loadMatrix(MatrixStack::Projection, m_pDefaultCamera->getViewProjectionMatrix());

    m_root->visit(renderer, eyeProjection, 0);

    renderer->draw();

    m_pGame->popMatrix(MatrixStack::Projection);

    //for (const auto& camera : getCameras()) {
    //    if (!camera->isVisible()) {
    //        continue;
    //    }

    //    Camera2D::s_pVisitingCamera = camera;

    //    const auto& transform = getNodeToParentTransform();

    //    m_pGame->pushMatrix(MatrixStack::Projection);
    //    m_pGame->loadMatrix(MatrixStack::Projection, Camera2D::s_pVisitingCamera->getViewProjectionMatrix());

    //    camera->apply();

    //    Node2D::visit(renderer, transform, 0);

    //    renderer->draw();

    //    m_pGame->popMatrix(MatrixStack::Projection);
    //}

    Camera2D::s_pVisitingCamera = nullptr;
}

void Scene::onEnter()
{
    OCFASSERT(m_root, "Scene root is not initialized.");
    m_root->onEnter();
}

void Scene::onExit()
{
    OCFASSERT(m_root, "Scene root is not initialized.");
    m_root->onExit();
}

const std::vector<Camera2D*>& Scene::getCameras()
{
    return m_cameras;
}

void Scene::addNode(Node* node)
{
  //  m_root->addChild(node);
}

void Scene::removeNode(Node* node)
{
    if (m_root) {
        m_root->removeChild(node);
    }
}

NS_OCF_END
