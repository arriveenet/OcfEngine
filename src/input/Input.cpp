#include "Input.h"
#include <cstring>
#include "base/Game.h"
#include "platform/RenderViewImpl.h"

namespace ocf {

Keyboard Input::m_keyboard;
Mouse Input::m_mouse;
Input::MouseMode Input::m_mouseMode = Input::MouseMode::Normal;

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

    RenderViewImpl* view = dynamic_cast<RenderViewImpl*>(Game::getInstance()->getRenderView());
    m_mouse.m_position = view->getMousePosition();

    // マウスボタンの状態を保持
    m_mouse.m_previousButton = m_mouse.m_currentButton;
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
    RenderViewImpl* view = static_cast<RenderViewImpl*>(Game::getInstance()->getRenderView());
    view->setCursorMode(mode);
}

Input::MouseMode Input::getMouseMode()
{
    return m_mouseMode;
}

} // namespace ocf
