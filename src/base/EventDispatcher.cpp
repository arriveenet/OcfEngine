#include "EventDispatcher.h"
#include "base/EventListener.h"

namespace {
std::string getListenerID(ocf::EventType type)
{
    switch (type)
    {
    case ocf::EventType::KeyboardEvent:
        return "KeyEvent";
    case ocf::EventType::MouseEvent:
        return "MouseEvent";
    default:
        break;
    }

    return "";
}
}

NS_OCF_BEGIN

EventDispatcher::EventLisnnerVector::EventLisnnerVector()
{
}

EventDispatcher::EventLisnnerVector::~EventLisnnerVector()
{
    while (!m_eventListeners.empty()) {
        delete m_eventListeners.back();
        m_eventListeners.pop_back();
    }
}

void EventDispatcher::EventLisnnerVector::clear()
{
    m_eventListeners.clear();
}

void EventDispatcher::EventLisnnerVector::emplace_back(EventListener* eventListener)
{
    m_eventListeners.emplace_back(eventListener);
}

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

void EventDispatcher::dispatchEvent(Event* pEvent)
{
    EventType type = pEvent->getEventType();
    auto listenerId = getListenerID(type);

    auto iter = m_listenerMap.find(listenerId);
    if (iter != m_listenerMap.end()) {
        auto& listeners = iter->second;
        for (auto& listener : listeners) {
            listener->m_onEvent(pEvent);
        }
    }
}

void EventDispatcher::addEventListener(EventListener* pEventListener, Node* pTarget)
{
    auto listenerId = pEventListener->getListenerId();
    m_listenerMap[listenerId].emplace_back(pEventListener);
}

NS_OCF_END
