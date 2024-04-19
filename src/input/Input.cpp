#include "Input.h"
#include <cstring>
#include "base/Game.h"

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
	// キーボードの状態を保持
	memcpy(m_inputState.keyboard.m_prevState, m_inputState.keyboard.m_currentState, KEY_CODE_MAX);

	// マウスの座標を設定
	m_inputState.mouse.m_position = Applicaiton::getInstance()->getCursorPosition();

	// マウスボタンの状態を保持
	m_inputState.mouse.m_previousButton = m_inputState.mouse.m_currentButton;
}

OCF_END
