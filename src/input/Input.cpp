#include "Input.h"
#include <cstring>
#include "base/Game.h"
#include "platform/GLViewImpl.h"

NS_OCF_BEGIN

Keyboard Input::m_keyboard;
Mouse Input::m_mouse;

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
    // �L�[�{�[�h�̏�Ԃ�ێ�
    memcpy(m_keyboard.m_prevState, m_keyboard.m_currentState, sizeof(m_keyboard.m_prevState));

    // �}�E�X�̍��W��ݒ�
    m_mouse.m_lastPosition = m_mouse.m_position;

    GLViewImpl* view = dynamic_cast<GLViewImpl*>(Game::getInstance()->getGLView());
    m_mouse.m_position = view->getMousePosition();

    // �}�E�X�{�^���̏�Ԃ�ێ�
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

NS_OCF_END
