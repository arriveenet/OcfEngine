#include "Scene.h"
#include "2d/Camera2D.h"
#include "3d/Camera3D.h"
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

    auto size = m_pGame->getVisibleSize();
    m_root->setSize(size);


}

Scene::~Scene()
{
    OCF_SAFE_DELETE(m_pDefaultCamera);
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
    updateScene(deltaTime);
    m_root->update(deltaTime);
}

void Scene::updateScene(float /*deltaTime*/)
{
}

void Scene::render(Renderer* renderer, const glm::mat4& eyeProjection )
{
    OCFASSERT(m_root, "Scene root is not initialized.");

    m_root->determineActiveCamera();

    Camera3D* camera3D = m_root->getCamera3D();
    if (camera3D) {
        Camera3D::s_pVisitingCamera = camera3D;
    }

    m_pGame->pushMatrix(MatrixStack::Projection);
    m_pGame->loadMatrix(MatrixStack::Projection, m_pDefaultCamera->getViewProjectionMatrix());

    m_root->visit(renderer, eyeProjection, 0);

    renderer->draw();

    m_pGame->popMatrix(MatrixStack::Projection);

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

void Scene::addNode(Node* /*node*/)
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
