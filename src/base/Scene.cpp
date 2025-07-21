#include "Scene.h"
#include "2d/Camera2D.h"
#include "base/Game.h"
#include "renderer/Renderer.h"

NS_OCF_BEGIN

Scene::Scene()
    : m_pDefaultCamera(nullptr)
{
    m_ignoreAnchorPointForPosition = true;
    setAnchorPoint(glm::vec2(0.5f, 0.5f));

    Camera2D::s_pVisitingCamera = nullptr;
}

Scene::~Scene()
{
}

bool Scene::init()
{
    m_pDefaultCamera = new Camera2D();
    m_pDefaultCamera->init();

    addChild(m_pDefaultCamera);

    auto size = m_pGame->getVisibleSize();
    setSize(size);

    return true;
}

void Scene::render(Renderer* renderer, const glm::mat4& /* eyeProjection */)
{
    for (const auto& camera : getCameras()) {
        if (!camera->isVisible()) {
            continue;
        }

        Camera2D::s_pVisitingCamera = camera;

        const auto& transform = getNodeToParentTransform();

        m_pGame->pushMatrix(MatrixStack::Projection);
        m_pGame->loadMatrix(MatrixStack::Projection, Camera2D::s_pVisitingCamera->getViewProjectionMatrix());

        camera->apply();

        Node2D::visit(renderer, transform, 0);

        renderer->draw();

        m_pGame->popMatrix(MatrixStack::Projection);
    }

    Camera2D::s_pVisitingCamera = nullptr;
}

const std::vector<Camera2D*>& Scene::getCameras()
{
    return m_cameras;
}

NS_OCF_END
