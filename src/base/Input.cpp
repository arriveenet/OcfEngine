#include "Input.h"
#include <cstring>

OCF_BEGIN

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
	memcpy(m_inputState.keyboard.m_prevState, m_inputState.keyboard.m_currentState, KEY_CODE_MAX);
}

OCF_END
