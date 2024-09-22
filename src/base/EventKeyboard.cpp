#include "EventKeyboard.h"

NS_OCF_BEGIN

EventKeyboard::EventKeyboard(Keyboard::KeyCode key, bool isPressed)
    : Event(EventType::KeyboardEvent)
    , m_keyCode(key)
    , m_isPressed(isPressed)
{
}

NS_OCF_END
