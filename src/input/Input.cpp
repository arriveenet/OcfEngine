#include "Input.h"
#include <cstring>
#include "base/Game.h"
#include "platform/GLViewImpl.h"

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
    // �L�[�{�[�h�̏�Ԃ�ێ�
    memcpy(m_inputState.keyboard.m_prevState, m_inputState.keyboard.m_currentState, sizeof(m_inputState.keyboard.m_prevState));

    // �}�E�X�̍��W��ݒ�
    GLViewImpl* view = dynamic_cast<GLViewImpl*>(Game::getInstance()->getGLView());
    m_inputState.mouse.m_position = view->getMousePosition();

    // �}�E�X�{�^���̏�Ԃ�ێ�
    m_inputState.mouse.m_previousButton = m_inputState.mouse.m_currentButton;
}

NS_OCF_END
