#pragma once
#include "base/Event.h"
#include "input/Keyboard.h"

NS_OCF_BEGIN

class EventKeyboard : public Event {
public:
    EventKeyboard(Keyboard::KeyCode key, bool isPressed);

    Keyboard::KeyCode m_keyCode;
    bool m_isPressed;
};

NS_OCF_END