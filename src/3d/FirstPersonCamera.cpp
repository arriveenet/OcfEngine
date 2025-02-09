#include "3d/FirstPersonCamera.h"
#include "2d/Node.h"
#include "input/Input.h"
#include "base/Game.h"
#include "base/EventListenerKeyboard.h"
#include "base/EventListenerMouse.h"
#include "base/EventDispatcher.h"
#include <glm/gtc/matrix_transform.hpp>

NS_OCF_BEGIN

FirstPersonCamera* FirstPersonCamera::create(float fovy, float aspect, float zNear, float zFar)
{
    FirstPersonCamera* pCamera = new FirstPersonCamera();
    if (pCamera->initPerspective(glm::radians(fovy), aspect, zNear, zFar)) {
        return pCamera;
    }

    return nullptr;
}

FirstPersonCamera::FirstPersonCamera()
    : m_moveSpeed(5.5f)
    , m_sensitivity(0.1f)
    , m_pitch(0.0f)
    , m_yaw(0.0f)
    , m_front(0.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_right(0.0f, 0.0f, 0.0f)
    , m_keyStates{ false }
    , m_isCameraControl(true)
{
}

FirstPersonCamera::~FirstPersonCamera()
{
    Input::setMouseMode(Input::MouseMode::Visible);
}

bool FirstPersonCamera::initPerspective(float fovy, float aspect, float zNear, float zFar)
{
    // �C�x���g���X�i�[�̓o�^
    auto mouseListener = EventListenerMouse::create();
    mouseListener->m_onEvent = std::bind(&FirstPersonCamera::onMouseMove, this, std::placeholders::_1);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->m_onKeyPressed = std::bind(&FirstPersonCamera::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
    keyboardListener->m_onKeyReleased = std::bind(&FirstPersonCamera::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);

    m_pGame->getEventDispatcher()->addEventListener(mouseListener, this);
    m_pGame->getEventDispatcher()->addEventListener(keyboardListener, this);

    // �}�E�X�J�[�\���𒆉���
    Input::setMouseMode(Input::MouseMode::Captured);

    return Camera::initPerspective(fovy, aspect, zNear, zFar);
}

void FirstPersonCamera::updateNode(float deltaTime)
{
    if (!m_isCameraControl) {
        return;
    }

    const float velocity = m_moveSpeed * deltaTime;

    if (m_keyStates[static_cast<int>(Movement::Forward)]) {
        m_position += m_front * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Backward)]) {
        m_position -= m_front * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Left)]) {
        m_position -= m_right * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Right)]) {
        m_position += m_right * velocity;
    }
}

void FirstPersonCamera::onMouseMove(Event* pEvent)
{
    if (!m_isCameraControl) {
        return;
    }

    EventMouse* mouseEvent = static_cast<EventMouse*>(pEvent);

    if (mouseEvent->m_mouseEventType == EventMouse::MouseEventType::Move) {
        const glm::vec2 pos = mouseEvent->getPosition();
        const glm::vec2 center = m_pGame->getVisibleSize() / 2.0f;

        float xOffset = pos.x - center.x;
        float yOffset = center.y - pos.y;

        xOffset *= m_sensitivity;
        yOffset *= m_sensitivity;

        m_yaw += xOffset;
        m_pitch += yOffset;

        // �s�b�`�p�x�̐���
        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
    }

    // �J�����̌������X�V
    updateCameraVectors();

}

void FirstPersonCamera::onKeyPressed(Keyboard::KeyCode key, Event* /* pEvent */)
{
    if (key == Keyboard::KeyCode::KEY_W) {
        m_keyStates[static_cast<int>(Movement::Forward)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_S) {
        m_keyStates[static_cast<int>(Movement::Backward)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_A) {
        m_keyStates[static_cast<int>(Movement::Left)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_D) {
        m_keyStates[static_cast<int>(Movement::Right)] = true;
    }
}

void FirstPersonCamera::onKeyReleased(Keyboard::KeyCode key, Event* /* pEvent */)
{
    if (key == Keyboard::KeyCode::KEY_W) {
        m_keyStates[static_cast<int>(Movement::Forward)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_S) {
        m_keyStates[static_cast<int>(Movement::Backward)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_A) {
        m_keyStates[static_cast<int>(Movement::Left)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_D) {
        m_keyStates[static_cast<int>(Movement::Right)] = false;
    }
}

const glm::mat4 FirstPersonCamera::getViewMatrix() const
{
    glm::mat4 view = glm::lookAt(m_position, m_position + m_front, glm::vec3(0.0f, 1.0f, 0.0f));
    if (view != m_view) {
        m_viewProjectionDirty = true;
        m_view = view;
    }

    return m_view;
}

void FirstPersonCamera::setCameraControl(bool centerCursor)
{
    m_isCameraControl = centerCursor;

    if (centerCursor) {
        Input::setMouseMode(Input::MouseMode::Captured);
    }
    else {
        Input::setMouseMode(Input::MouseMode::Visible);
    }
}

void FirstPersonCamera::updateCameraVectors()
{
    glm::vec3 front{};
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

NS_OCF_END