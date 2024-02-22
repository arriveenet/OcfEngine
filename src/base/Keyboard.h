#pragma once
#include "types.h"

#define KEY_CODE_MAX	34

struct GLFWwindow;

OCF_BEGIN

class Keyboard {
public:
	friend class Input;
	static void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool init();

	ButtonState getKeyState(int key) const;
private:
	static uint8_t m_currentState[KEY_CODE_MAX];
	static uint8_t m_prevState[KEY_CODE_MAX];
};

OCF_END
