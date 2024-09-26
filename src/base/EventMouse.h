#pragma once
#include <glm/glm.hpp>
#include "base/Event.h"
#include "input/Mouse.h"

NS_OCF_BEGIN

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

    void setPosition(float x, float y) { m_position.x = x, m_position.y = y; }
    void setPosition(const glm::vec2& position) { m_position = position; }
    glm::vec2 getPosition() const { return m_position; }


    MouseEventType m_mouseEventType;
    Mouse::MouseButton m_mouseButton;
    glm::vec2 m_position;
};

NS_OCF_END