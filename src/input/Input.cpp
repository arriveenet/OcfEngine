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
	// �L�[�{�[�h�̏�Ԃ�ێ�
	memcpy(m_inputState.keyboard.m_prevState, m_inputState.keyboard.m_currentState, KEY_CODE_MAX);

	// �}�E�X�̍��W��ݒ�
	m_inputState.mouse.m_position = Applicaiton::getInstance()->getCursorPosition();

	// �}�E�X�{�^���̏�Ԃ�ێ�
	m_inputState.mouse.m_previousButton = m_inputState.mouse.m_currentButton;
}

OCF_END
