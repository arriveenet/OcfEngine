#include "Camera2D.h"
#include <assert.h>
#include <glm/gtc/matrix_transform.hpp>
#include "platform/Application.h"
#include "base/Game.h"
#include "renderer/Renderer.h"

NS_OCF_BEGIN

Camera2D* Camera2D::s_pVisitingCamera = nullptr;
glm::vec4 Camera2D::s_defaultViewport = glm::vec4(0.0f);

Camera2D* Camera2D::createOrthographic(float width, float height, float zNear /*=-1.0f*/, float zFar /*= 1.0f*/)
{
    Camera2D* pCamera = new Camera2D();
    pCamera->initOrthographic(width, height, zNear, zFar);

    return pCamera;
}

Camera2D* Camera2D::getDefaultCamera()
{
    Scene* pScene = Game::getInstance()->getCurrentScene();
    assert(pScene != nullptr);

    return pScene->getDefaultCamera();
}

Camera2D* Camera2D::getVisitingCamera()
{
    return s_pVisitingCamera;
}

const glm::vec4& Camera2D::getDefaultViewpot()
{
    return s_defaultViewport;
}

void Camera2D::setDefaultViewport(const glm::vec4& viewport)
{
    s_defaultViewport = viewport;
}

Camera2D::Camera2D()
    : m_cameraFlag(CameraFlag::Default)
    , m_projection(1.0f)
    , m_center(0.0f)
    , m_view(1.0f)
    , m_viewInverse(1.0f)
    , m_viewProjection(1.0f)
    , m_zNear(0.0f)
    , m_zFar(0.0f)
    , m_scene(nullptr)
    , m_viewProjectionDirty(true)
{
    setName("Camera2D");
}

Camera2D::~Camera2D()
{
}

bool Camera2D::init()
{
    glm::vec2 size = m_pGame->getResolutionSize();
    m_zNear = -1024.0f;
    m_zFar = 1024.0f;

    const float halfWidth = size.x / 2.0f;
    const float halfHeight = size.y / 2.0f;

    initOrthographic(-halfWidth, halfWidth, halfHeight, -halfHeight, m_zNear, m_zFar);
    glm::vec3 eye(size.x / 2.0f, size.y / 2.0f, 1.0f);
    glm::vec3 center(size.x / 2.0f, size.y / 2.0f, 0.0f);
    setPosition(eye);
    lookAt(center, glm::vec3(0.0f, 1.0f, 0.0f));

    return true;
}

bool Camera2D::initOrthographic(float left, float right, float bottom, float top, float zNear,
                              float zFar)
{
    m_projection = glm::ortho(left, right, bottom, top, zNear, zFar);

    m_viewProjectionDirty = true;

    return true;
}

void Camera2D::lookAt(const glm::vec3& center, const glm::vec3& up)
{
    m_center = center;
    m_view = glm::lookAt(glm::vec3(m_position, 1.0f), center, up);
}

const glm::mat4 Camera2D::getProjectionMatrix() const
{
    return m_projection;
}

const glm::mat4 Camera2D::getViewMatrix() const
{
    //glm::mat4 viewInv(getNodeToWorldTransform());
    //if (viewInv != m_viewInverse) {
    //	m_viewInverse = viewInv;
    //	m_view = glm::inverse(viewInv);
    //}
    glm::mat4 view =
        glm::lookAt(glm::vec3(m_position, 1.0f), m_center, glm::vec3(0.0f, 1.0f, 0.0f));
    if (view != m_view) {
        m_viewProjectionDirty = true;
        m_view = view;
    }

    return m_view;
}

const glm::mat4 Camera2D::getViewProjectionMatrix() const
{
    getViewMatrix();
    if (m_viewProjectionDirty) {
        m_viewProjectionDirty = false;
        m_viewProjection = m_projection * m_view;
    }

    return m_viewProjection;
}

void Camera2D::onEnter()
{
    if (m_scene == nullptr) {
        auto scene = getScene();
        if (scene) {
            // シーンにこのカメラを追加
            setScene(scene);
        }
    }
    Node::onEnter();
}

void Camera2D::onExit()
{
    // シーンからこのカメラを削除
    setScene(nullptr);
    Node::onExit();
}

void Camera2D::apply()
{
    m_pGame->getRenderer()->setViewPort(static_cast<int>(s_defaultViewport.x),
                                        static_cast<int>(s_defaultViewport.y),
                                        static_cast<int>(s_defaultViewport.z),
                                        static_cast<int>(s_defaultViewport.w));
}

void Camera2D::setScene(Scene* scene)
{
    if (m_scene != scene) {
        // シーンからこのカメラを削除
        if (scene == nullptr) {
            auto& cameras = m_scene->m_cameras;
            auto iter = std::find(cameras.begin(), cameras.end(), this);
            if (iter != cameras.end()) {
                cameras.erase(iter);
            }
            m_scene = nullptr;
        }
        // シーンにこのカメラを追加
        else {
            m_scene = scene;
            auto& cameras = m_scene->m_cameras;
            auto iter = std::find(cameras.begin(), cameras.end(), this);
            if (iter == cameras.end()) {
                cameras.emplace_back(this);
            }
        }
    }
}

glm::vec3 Camera2D::unProjectGL(const glm::vec3& src) const
{
    const glm::vec2& size = m_pGame->getResolutionSize();
    glm::vec4 viewport(0.0f, 0.0f, size.x, size.y);

    return glm::unProject(src, m_view, m_projection, viewport);
}

NS_OCF_END
