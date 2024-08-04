#include "Mouse.h"
#include <GLFW/glfw3.h>

NS_OCF_BEGIN

uint32_t Mouse::m_currentButton = 0;
uint32_t Mouse::m_previousButton = 0;

void Mouse::onMouseButton(int button, int action, int mods)
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
	// �O�̃{�^����������Ă��Ȃ��ꍇ
	if ((m_previousButton & button) == 0) {
		// ���݂̃{�^����������Ă��Ȃ��ꍇ
		if ((m_currentButton & button) == 0) {
			return ButtonState::None;
		}
		// ���݂̃{�^����������Ă���ꍇ
		else {
			return ButtonState::Pressed;
		}
	}
	// �O�̃{�^����������Ă���ꍇ
	else {
		// ���݂̃{�^����������Ă��Ȃ��ꍇ
		if ((m_currentButton & button) == 0) {
			return ButtonState::Released;
		}
		// ���݂̃{�^����������Ă���ꍇ
		else {
			return ButtonState::Hold;
		}
	}
}

NS_OCF_END
