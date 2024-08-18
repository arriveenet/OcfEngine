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
    // キーボードの状態を保持
    memcpy(m_inputState.keyboard.m_prevState, m_inputState.keyboard.m_currentState, sizeof(m_inputState.keyboard.m_prevState));

    // マウスの座標を設定
    GLViewImpl* view = dynamic_cast<GLViewImpl*>(Game::getInstance()->getGLView());
    m_inputState.mouse.m_position = view->getMousePosition();

    // マウスボタンの状態を保持
    m_inputState.mouse.m_previousButton = m_inputState.mouse.m_currentButton;
}

NS_OCF_END
