#pragma once
#include <glm/glm.hpp>
#include "base/Event.h"
#include "input/Mouse.h"

namespace ocf {

class EventMouse : public Event {
public:
    enum class MouseEventType {
        None,
        Down,
        Up,
        Move,
        Scroll
    };

    EventMouse(MouseEventType mouseEventType);

    void setMouseButton(Mouse::MouseButton button) { m_mouseButton = button; }
    Mouse::MouseButton getMouseButton() const { return m_mouseButton; }

    void setPosition(const glm::vec2& position) { m_position = position; }
    void setLastPosition(const glm::vec2& position) { m_lastPosition = position; }
    glm::vec2 getLastPosition() const { return m_lastPosition; }
    glm::vec2 getPosition() const { return m_position; }
    glm::vec2 getDelta() const;

    void setScrollDelta(const glm::vec2& scrollDelta) { m_scrollDelta = scrollDelta; }
    glm::vec2 getScrollDelta() const { return m_scrollDelta; }

    MouseEventType m_mouseEventType;
    Mouse::MouseButton m_mouseButton;
    glm::vec2 m_position;
    glm::vec2 m_lastPosition;
    glm::vec2 m_scrollDelta;
};

} // namespace ocf