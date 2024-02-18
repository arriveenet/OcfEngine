#include "Keyboard.h"
#include <cstring>
#include <cstdio>

uint8_t Keyboard::m_currentState[GLFW_KEY_LAST];
uint8_t Keyboard::m_prevState[GLFW_KEY_LAST];

void Keyboard::onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_currentState[key] = action;
}

bool Keyboard::init()
{
	memset(m_currentState, GLFW_RELEASE, sizeof(m_currentState));
	memset(m_prevState, GLFW_RELEASE, sizeof(m_prevState));

	return true;
}

ButtonState Keyboard::getKeyState(int key) const
{
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
			return ButtonState::Held;
		}
	}
}
