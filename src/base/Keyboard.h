#pragma once
#include <GLFW/glfw3.h>
#include "types.h"

class Keyboard {
public:
	friend class Input;
	static void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool init();

	ButtonState getKeyState(int key) const;
private:
	static uint8_t m_currentState[GLFW_KEY_LAST];
	static uint8_t m_prevState[GLFW_KEY_LAST];
};