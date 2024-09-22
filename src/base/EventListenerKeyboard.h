#pragma once
#include "base/EventListener.h"
#include "input/Keyboard.h"

NS_OCF_BEGIN

class EventListenerKeyboard : public EventListener {
public:
    static EventListenerKeyboard* create();

    EventListenerKeyboard();

    bool init();

    std::string getListenerId() override;

    std::function<void(Keyboard::KeyCode, Event*)> m_onKeyPressed;
    std::function<void(Keyboard::KeyCode, Event*)> m_onKeyReleased;
};

NS_OCF_END
