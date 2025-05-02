#include "UIWidget.h"
#include "2d/Camera.h"
#include "base/EventDispatcher.h"
#include "base/EventListenerMouse.h"
#include "base/EventMouse.h"
#include "base/Game.h"
#include "base/Rect.h"

NS_OCF_BEGIN

namespace ui {

Widget::Widget()
    : m_eventListenerMouse(nullptr)
    , m_mouseEnabled(false)
{
}
Widget::~Widget()
{
    if (m_mouseEnabled) {
        m_pEventDispatcher->removeEventListener(m_eventListenerMouse);
        OCF_SAFE_RELEASE(m_eventListenerMouse);
        m_eventListenerMouse = nullptr;
    }
}

bool Widget::init()
{
    if (Node::init()) {
        initRenderer();
        setAnchorPoint(glm::vec2(0.5f, 0.5f));

        return true;
    }
    return false;
}

void Widget::setMouseEnabled(bool enabled)
{
    if (m_mouseEnabled == enabled) {
        return;
    }

    m_mouseEnabled = enabled;

    if (m_mouseEnabled) {
        m_eventListenerMouse = EventListenerMouse::create();
        m_eventListenerMouse->m_onMouseUp = std::bind(&Widget::onMouseUp,
                                                      this, std::placeholders::_1);
        m_eventListenerMouse->m_onMouseDown = std::bind(&Widget::onMouseDown,
                                                        this, std::placeholders::_1);
        m_eventListenerMouse->m_onMouseMove = std::bind(&Widget::onMouseMove,
                                                        this, std::placeholders::_1);
        m_pEventDispatcher->addEventListener(m_eventListenerMouse, this);
    }
    else {
        m_pEventDispatcher->removeEventListener(m_eventListenerMouse);
        OCF_SAFE_RELEASE(m_eventListenerMouse);
        m_eventListenerMouse = nullptr;
    }
}

bool Widget::hitTest(const glm::vec2& point, const Camera* camera, glm::vec3* p) const
{
    Rect rect;
    rect.m_size = getSize();
    return isScreenPointInRect(point, camera, getWorldToNodeTransform(), rect, p);
}

void Widget::onMouseUp(Event* event)
{
    m_isHit = false;

    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    auto mousePos = mouseEvent->getPosition();
    mousePos.y = m_pGame->getVisibleSize().y - mousePos.y;
    auto camera = Camera::getVisitingCamera();
    if (hitTest(mousePos, camera, nullptr)) {
        m_isHit = true;
    }

    if (!m_isHit) {
        return;
    }

    if (m_onClickCallback) {
        m_onClickCallback();
    }
}

void Widget::onMouseDown(Event* /*event*/)
{
}

void Widget::onMouseMove(Event* event)
{
    m_isHit = false;

    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    auto mousePos = mouseEvent->getPosition();
    auto camera = Camera::getVisitingCamera();
    if (hitTest(mousePos, camera, nullptr)) {
        m_isHit = true;
    }
}

void Widget::initRenderer()
{
}

}

NS_OCF_END