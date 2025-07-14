#include "Camera3D.h"
#include "base/Game.h"
#include "platform/Application.h"
#include "renderer/Renderer.h"
#include <assert.h>
#include <glm/gtc/matrix_transform.hpp>

NS_OCF_BEGIN

Camera3D::Camera3D()
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

Camera3D* Camera3D::createPerspective(float fovy, float aspect, float zNear, float zFar)
{
    Camera3D* pCamera3D = new Camera3D();
    pCamera3D->initPerspective(glm::radians(fovy), aspect, zNear, zFar);

    return pCamera3D;
}

Camera3D* Camera3D::createOrthographic(float width, float height, float zNear /*=-1.0f*/,
                                   float zFar /*= 1.0f*/)
{
    Camera3D* pCamera3D = new Camera3D();
    pCamera3D->initOrthographic(width, height, zNear, zFar);

    return pCamera3D;
}

Camera3D::~Camera3D()
{
}

bool Camera3D::init()
{
    glm::vec2 size = m_pGame->getResolutionSize();
    switch (m_pGame->getProjection()) {
    case Game::Projection::_3D: {
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
    case Game::Projection::_2D: {
        m_zNear = -1024.0f;
        m_zFar = 1024.0f;

        const float halfWidth = size.x / 2.0f;
        const float halfHeight = size.y / 2.0f;

        initOrthographic(-halfWidth, halfWidth, halfHeight, -halfHeight, m_zNear, m_zFar);
        glm::vec3 eye(size.x / 2.0f, size.y / 2.0f, 1.0f);
        glm::vec3 center(size.x / 2.0f, size.y / 2.0f, 0.0f);
        setPosition(eye);
        lookAt(center, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    }
    }

    return true;
}

bool Camera3D::initPerspective(float fovy, float aspect, float zNear, float zFar)
{
    m_type = Type::Perspective;

    m_projection = glm::perspective(fovy, aspect, zNear, zFar);

    m_viewProjectionDirty = true;

    return true;
}

bool Camera3D::initOrthographic(float left, float right, float bottom, float top, float zNear,
                              float zFar)
{
    m_type = Type::Orthographic;

    m_projection = glm::ortho(left, right, bottom, top, zNear, zFar);

    m_viewProjectionDirty = true;

    return true;
}

void Camera3D::lookAt(const glm::vec3& center, const glm::vec3& up)
{
    m_center = center;
    m_view = glm::lookAt(m_position, center, up);
}

const glm::mat4 Camera3D::getProjectionMatrix() const
{
    return m_projection;
}

const glm::mat4 Camera3D::getViewMatrix() const
{
    // glm::mat4 viewInv(getNodeToWorldTransform());
    // if (viewInv != m_viewInverse) {
    //	m_viewInverse = viewInv;
    //	m_view = glm::inverse(viewInv);
    // }
    glm::mat4 view = glm::lookAt(m_position, m_center, glm::vec3(0.0f, 1.0f, 0.0f));
    if (view != m_view) {
        m_viewProjectionDirty = true;
        m_view = view;
    }

    return m_view;
}

const glm::mat4 Camera3D::getViewProjectionMatrix() const
{
    getViewMatrix();
    if (m_viewProjectionDirty) {
        m_viewProjectionDirty = false;
        m_viewProjection = m_projection * m_view;
    }

    return m_viewProjection;
}

glm::vec3 Camera3D::unProjectGL(const glm::vec3& src) const
{
    const glm::vec2& size = m_pGame->getResolutionSize();
    glm::vec4 viewport(0.0f, 0.0f, size.x, size.y);

    return glm::unProject(src, m_view, m_projection, viewport);
}

NS_OCF_END
