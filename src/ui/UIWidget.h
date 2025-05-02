#pragma once
#include "2d/Node.h"
#include <functional>

NS_OCF_BEGIN

class EventDispatcher;
class EventListenerMouse;
class Event;

namespace ui {

using ClickCallback = std::function<void()>;

class Widget : public Node {
public:
    Widget();
    virtual ~Widget();

    virtual bool init();

    virtual void setMouseEnabled(bool enabled);

    virtual bool isMouseEnabled() const { return m_mouseEnabled; }

    virtual bool hitTest(const glm::vec2& point, const Camera* camera, glm::vec3* p) const;

    virtual void onMouseUp(Event* event);

    virtual void onMouseDown(Event* event);

    virtual void onMouseMove(Event* event);

protected:
    virtual void initRenderer();

protected:
    EventListenerMouse* m_eventListenerMouse;
    bool m_mouseEnabled;
    bool m_isHit;
    ClickCallback m_onClickCallback;
};

}

NS_OCF_END