#include "EventMouse.h"

NS_OCF_BEGIN

EventMouse::EventMouse(MouseEventType mouseEventType)
    : Event(EventType::MouseEvent)
    , m_mouseEventType(mouseEventType)
    , m_mouseButton(Mouse::Button1)
    , m_position(0.0f, 0.0f)
    , m_lastPosition(0.0f, 0.0f)
{
}

glm::vec2 EventMouse::getDelta() const
{
    const float deltaX = m_position.x - m_lastPosition.x;
    const float deltaY = m_lastPosition.y - m_position.y;
    return glm::vec2(deltaX, deltaY);
}

NS_OCF_END
