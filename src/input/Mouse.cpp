#include "Mouse.h"
#include <GLFW/glfw3.h>

OCF_BEGIN

uint32_t Mouse::m_currentButton = 0;
uint32_t Mouse::m_previousButton = 0;

void Mouse::onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	constexpr int offset = MouseButton::Button1 - GLFW_MOUSE_BUTTON_1;
	button += offset;

	if (action == GLFW_PRESS) {
		m_currentButton |= button;
	} else {
		m_currentButton &= ~button;
	}
}

Mouse::Mouse()
	: m_position(0.0f, 0.0f)
{
}

ButtonState Mouse::getButtonState(MouseButton button) const
{
	// 前のボタンが押されていない場合
	if ((m_previousButton & button) == 0) {
		// 現在のボタンが押されていない場合
		if ((m_currentButton & button) == 0) {
			return ButtonState::None;
		}
		// 現在のボタンが押されている場合
		else {
			return ButtonState::Pressed;
		}
	}
	// 前のボタンが押されている場合
	else {
		// 現在のボタンが押されていない場合
		if ((m_currentButton & button) == 0) {
			return ButtonState::Released;
		}
		// 現在のボタンが押されている場合
		else {
			return ButtonState::Hold;
		}
	}
}

OCF_END
