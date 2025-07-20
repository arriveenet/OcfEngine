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
    // イベントリスナーの登録
    auto mouseListener = EventListenerMouse::create();
    mouseListener->m_onEvent = std::bind(&FirstPersonCamera::onMouseMove, this, std::placeholders::_1);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->m_onKeyPressed = std::bind(&FirstPersonCamera::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
    keyboardListener->m_onKeyReleased = std::bind(&FirstPersonCamera::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);

    m_pGame->getEventDispatcher()->addEventListener(mouseListener, this);
    m_pGame->getEventDispatcher()->addEventListener(keyboardListener, this);

    // マウスカーソルを中央に
    Input::setMouseMode(Input::MouseMode::Captured);

    return Camera::initPerspective(fovy, aspect, zNear, zFar);
}

void FirstPersonCamera::updateNode(float deltaTime)
{
    if (!m_isCameraControl) {
        return;
    }

    const float velocity = m_moveSpeed * deltaTime;
    const glm::vec3 direction = glm::vec3(m_front.x, 0.0f, m_front.z);

    if (m_keyStates[static_cast<int>(Movement::Forward)]) {
        m_position += direction * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Backward)]) {
        m_position -= direction * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Left)]) {
        m_position -= m_right * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Right)]) {
        m_position += m_right * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Up)]) {
        m_position.y += 1.0f * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Down)]) {
        m_position.y -= 1.0f * velocity;
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

        // ヨー角度の制限
        if (m_yaw > 360.0f || m_yaw < -360.0f) {
            m_yaw = 0.0f;
        }

        // ピッチ角度の制限
        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
    }

    // カメラの向きを更新
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
    else if (key == Keyboard::KeyCode::KEY_SPACE) {
        m_keyStates[static_cast<int>(Movement::Up)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_X) {
        m_keyStates[static_cast<int>(Movement::Down)] = true;
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
    else if (key == Keyboard::KeyCode::KEY_SPACE) {
        m_keyStates[static_cast<int>(Movement::Up)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_X) {
        m_keyStates[static_cast<int>(Movement::Down)] = false;
    }
}

const glm::mat4 FirstPersonCamera::getViewMatrix() const
{
    glm::mat4 view = glm::lookAt(m_position, m_position + m_front, m_worldUp);
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
    front.x = cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
    front.y = sinf(glm::radians(m_pitch));
    front.z = sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

NS_OCF_END