#include "Input.h"
#include <cstring>
#include "base/Game.h"

NS_OCF_BEGIN

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
	auto app = Applicaiton::getInstance();
	auto windowSize = app->getWindowSize();
	auto cursorPos = app->getCursorPosition();
	float cursorX = cursorPos.x;
	float cursorY = windowSize.y - cursorPos.y;
	m_inputState.mouse.m_position = { cursorX, cursorY };

	// マウスボタンの状態を保持
	m_inputState.mouse.m_previousButton = m_inputState.mouse.m_currentButton;
}

NS_OCF_END
