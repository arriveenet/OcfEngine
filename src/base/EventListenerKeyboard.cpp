#include "EventListenerKeyboard.h"
#include "base/EventKeyboard.h"

NS_OCF_BEGIN

EventListenerKeyboard* EventListenerKeyboard::create()
{
    auto listener = new EventListenerKeyboard();
    if (listener->init()) {
        listener->autorelease();
        return listener;
    }
    OCF_SAFE_DELETE(listener);
    return nullptr;
}

EventListenerKeyboard::EventListenerKeyboard()
{
}

bool EventListenerKeyboard::init()
{
    auto listener = [this](Event* event) {
        EventKeyboard* keyboardEvent = static_cast<EventKeyboard*>(event);
        if (keyboardEvent->m_isPressed) {
            if (m_onKeyPressed) {
                m_onKeyPressed(keyboardEvent->m_keyCode, event);
            }
        }
        else {
            m_onKeyPressed(keyboardEvent->m_keyCode, event);
        }
    };

    if (EventListener::init(Type::Keyboard, listener)) {
        return true;
    }
    return false;
}

std::string EventListenerKeyboard::getListenerId()
{
    return "KeyEvent";
}

NS_OCF_END
