#include "Keyboard.h"
#include "renderer/OpenGLInclude.h"
#include <cstring>
#include <cstdio>

NS_OCF_BEGIN

uint8_t Keyboard::m_currentState[static_cast<int>(KeyCode::KEY_LAST)];
uint8_t Keyboard::m_prevState[static_cast<int>(KeyCode::KEY_LAST)];

void Keyboard::onKeyEvent(KeyCode key, int action)
{
    m_currentState[static_cast<int>(key)] = action;
}

bool Keyboard::init()
{
    memset(m_currentState, GLFW_RELEASE, sizeof(m_currentState));
    memset(m_prevState, GLFW_RELEASE, sizeof(m_prevState));

    return true;
}

ButtonState Keyboard::getKeyState(KeyCode keyCode) const
{
    const int key = static_cast<int>(keyCode);

    if (m_prevState[key] == GLFW_RELEASE) {
        if (m_currentState[key] == GLFW_RELEASE) {
            return ButtonState::None;
        }
        else {
            return ButtonState::Pressed;
        }
    }
    else {
        if (m_currentState[key] == GLFW_RELEASE) {
            return ButtonState::Released;
        }
        else {
            return ButtonState::Hold;
        }
    }
}

NS_OCF_END
