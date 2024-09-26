#include "EventMouse.h"

NS_OCF_BEGIN

EventMouse::EventMouse(MouseEventType mouseEventType)
    : Event(EventType::MouseEvent)
    , m_mouseEventType(mouseEventType)
    , m_mouseButton(Mouse::Button1)
    , m_position(0.0f, 0.0f)
{
}

NS_OCF_END
