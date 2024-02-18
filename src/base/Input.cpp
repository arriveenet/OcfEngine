#include "Input.h"
#include <cstring>

bool Input::init()
{
	m_inputState.keyboard.init();

	return true;
}

void Input::prepareUpdate()
{

}

void Input::update()
{
	memcpy(m_inputState.keyboard.m_prevState, m_inputState.keyboard.m_currentState, GLFW_KEY_LAST);
}
