#pragma once
#include "base/EventListener.h"
#include "base/EventMouse.h"

NS_OCF_BEGIN

class EventListenerMouse : public EventListener {
public:
    static EventListenerMouse* create();

    EventListenerMouse();

    bool init();

    std::string getListenerId() override;

    std::function<void(Event*)> m_onMouseUp;
    std::function<void(Event*)> m_onMouseDown;
    std::function<void(Event*)> m_onMouseMove;
    std::function<void(Event*)> m_onMouseScroll;
};

NS_OCF_END