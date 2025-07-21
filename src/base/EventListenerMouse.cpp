#include "EventListenerMouse.h"
#include "base/EventMouse.h"

NS_OCF_BEGIN

EventListenerMouse* EventListenerMouse::create()
{
    EventListenerMouse* event = new EventListenerMouse();
    if (event->init()) {
        event->autorelease();
        return event;
    }
    OCF_SAFE_DELETE(event);
    return nullptr;
}

EventListenerMouse::EventListenerMouse()
{
}

bool EventListenerMouse::init()
{
    auto listener = [this](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        switch (mouseEvent->m_mouseEventType) {
        case EventMouse::MouseEventType::Up:
            if (m_onMouseUp)
                m_onMouseUp(event);
            break;
        case EventMouse::MouseEventType::Down:
            if (m_onMouseDown)
                m_onMouseDown(event);
            break;
        case EventMouse::MouseEventType::Move:
            if (m_onMouseMove)
                m_onMouseMove(event);
            break;
        case EventMouse::MouseEventType::Scroll:
            // @Todo Add mouse scroll event
            //if (m_onMouseScroll)
            //    m_onMouseScroll;
            break;
        default:
            break;
        }
        };

    if (EventListener::init(Type::Mouse, listener)) {
        return true;
    }

    return false;
}

std::string EventListenerMouse::getListenerId()
{
    return "MouseEvent";
}

NS_OCF_END
