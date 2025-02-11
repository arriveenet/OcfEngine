#include "Input.h"
#include <cstring>
#include "base/Game.h"
#include "platform/GLViewImpl.h"

NS_OCF_BEGIN

Keyboard Input::m_keyboard;
Mouse Input::m_mouse;
Input::MouseMode Input::m_mouseMode = Input::MouseMode::Visible;

bool Input::init()
{
    m_keyboard.init();

    return true;
}

void Input::prepareUpdate()
{

}

void Input::update()
{
    // キーボードの状態を保持
    memcpy(m_keyboard.m_prevState, m_keyboard.m_currentState, sizeof(m_keyboard.m_prevState));

    // マウスの座標を設定
    m_mouse.m_lastPosition = m_mouse.m_position;

    GLViewImpl* view = dynamic_cast<GLViewImpl*>(Game::getInstance()->getGLView());
    m_mouse.m_position = view->getMousePosition();

    // マウスボタンの状態を保持
    m_mouse.m_previousButton = m_mouse.m_currentButton;

    if (m_mouseMode == Input::MouseMode::Captured) {
        glm::vec2 windowSize = view->getWindowSize();
        view->setCursolPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    }
}

ButtonState Input::getKeyState(Keyboard::KeyCode keyCode)
{
    return m_keyboard.getKeyState(keyCode);
}

glm::vec2 Input::getMousePosition()
{
    return m_mouse.getPosition();
}

ButtonState Input::getMouseButtonState(Mouse::MouseButton button)
{
    return m_mouse.getButtonState(button);
}

void Input::setMouseMode(MouseMode mode)
{
    m_mouseMode = mode;
}

Input::MouseMode Input::getMouseMode()
{
    return m_mouseMode;
}

NS_OCF_END
