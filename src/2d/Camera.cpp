#include "Camera.h"
#include <assert.h>
#include <glm/gtc/matrix_transform.hpp>
#include "platform/Application.h"
#include "base/Game.h"
#include "renderer/Renderer.h"

NS_OCF_BEGIN

Camera* Camera::s_pVisitingCamera = nullptr;
glm::vec4 Camera::s_defaultViewport = glm::vec4(0.0f);

Camera::Camera()
    : m_cameraFlag(CameraFlag::Default)
    , m_projection(1.0f)
    , m_center(0.0f)
    , m_view(1.0f)
    , m_viewInverse(1.0f)
    , m_viewProjection(1.0f)
    , m_zNear(0.0f)
    , m_zFar(0.0f)
    , m_type(Type::Perspective)
    , m_scene(nullptr)
    , m_viewProjectionDirty(true)
{
}

Camera* Camera::createPerspective(float fovy, float aspect, float zNear, float zFar)
{
    Camera* pCamera = new Camera();
    pCamera->initPerspective(glm::radians(fovy), aspect, zNear, zFar);

    return pCamera;
}

Camera* Camera::createOrthographic(float left, float right, float bottom, float top, float zNear /*=-1.0f*/, float zFar /*= 1.0f*/)
{
    Camera* pCamera = new Camera();
    pCamera->initOrthographic(left, right, zNear, zFar);

    return pCamera;
}

Camera* Camera::getDefaultCamera()
{
    Scene* pScene = Game::getInstance()->getCurrentScene();
    assert(pScene != nullptr);

    return pScene->getDefaultCamera();
}

Camera* Camera::getVisitingCamera()
{
    return s_pVisitingCamera;
}

const glm::vec4& Camera::getDefaultViewpot()
{
    return s_defaultViewport;
}

void Camera::setDefaultViewport(const glm::vec4& viewport)
{
    s_defaultViewport = viewport;
}

Camera::~Camera()
{
}

bool Camera::init()
{
    glm::ivec2 size = m_pGame->getResolutionSize();
    switch (m_pGame->getProjection()) {
        case Game::Projection::_3D:
        {
            float zEye = m_pGame->getZEye();
            m_zNear = 0.5f;
            m_zFar = zEye + size.y / 2.0f;
            initPerspective(glm::radians(60.0f), (float)size.x / size.y, m_zNear, m_zFar);
            glm::vec3 eye(size.x / 2.0f, size.y / 2.0f, zEye);
            glm::vec3 center(size.x / 2.0f, size.y / 2.0f, 0.0f);
            setPosition(eye);
            lookAt(center, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        }
        case Game::Projection::_2D:
        {
            m_zNear = -1024.0f;
            m_zFar  = 1024.0f;

            initOrthographic((float)size.x, (float)size.y, m_zNear, m_zFar);
            setPosition(glm::vec3(size.x / 2.0f, size.y / 2.0f, 0.0f));
            break;
        }
    }

    return true;
}

bool Camera::initPerspective(float fovy, float aspect, float zNear, float zFar)
{
    m_type = Type::Perspective;

    m_projection = glm::perspective(fovy, aspect, zNear, zFar);

    m_viewProjectionDirty = true;

    return true;
}

bool Camera::initOrthographic(float width, float height, float zNear /*=-1.0f*/, float zFar /*= 1.0f*/)
{
    m_type = Type::Orthographic;

    const float halfWidth = width / 2.0f;
    const float halfHeight = height / 2.0f;
    m_projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, zNear, zFar);

    m_viewProjectionDirty = true;

    return true;
}

void Camera::lookAt(const glm::vec3& center, const glm::vec3& up)
{
    m_center = center;
    m_view = glm::lookAt(m_position, center, up);
}

const glm::mat4 Camera::getProjectionMatrix() const
{
    return m_projection;
}

const glm::mat4 Camera::getViewMatrix() const
{
    //glm::mat4 viewInv(getNodeToWorldTransform());
    //if (viewInv != m_viewInverse) {
    //	m_viewInverse = viewInv;
    //	m_view = glm::inverse(viewInv);
    //}
    glm::mat4 view = glm::lookAt(m_position, m_center, glm::vec3(0.0f, 1.0f, 0.0f));
    if (view != m_view) {
        m_viewProjectionDirty = true;
        m_view = view;
    }

    return m_view;
}

const glm::mat4 Camera::getViewProjectionMatrix() const
{
    getViewMatrix();
    if (m_viewProjectionDirty) {
        m_viewProjectionDirty = false;
        m_viewProjection = m_projection * m_view;
    }

    return m_viewProjection;
}

void Camera::onEnter()
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

void Camera::onExit()
{
    // シーンからこのカメラを削除
    setScene(nullptr);
    Node::onExit();
}

void Camera::apply()
{
    m_pGame->getRenderer()->setViewPort(static_cast<int>(s_defaultViewport.x),
                                        static_cast<int>(s_defaultViewport.y),
                                        static_cast<int>(s_defaultViewport.z),
                                        static_cast<int>(s_defaultViewport.w));
}

void Camera::setScene(Scene* scene)
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

glm::vec3 Camera::unProjectGL(const glm::vec3& src) const
{
    const glm::vec2& size = m_pGame->getResolutionSize();
    glm::vec4 viewport(0.0f, 0.0f, size.x, size.y);

    return glm::unProject(src, m_view, m_projection, viewport);
}

NS_OCF_END
