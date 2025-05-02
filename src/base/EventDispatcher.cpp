#include "EventDispatcher.h"

#include "2d/Camera.h"
#include "2d/Node.h"
#include "2d/Scene.h"
#include "base/EventListener.h"
#include "base/Game.h"

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

    auto pfnDispatchEventToListeners = &EventDispatcher::dispatchEventToListeners;
    if (type == EventType::MouseEvent) {
        pfnDispatchEventToListeners = &EventDispatcher::dispatchMouseEventToListeners;
    }

    auto iter = m_listenerMap.find(listenerId);
    if (iter != m_listenerMap.end()) {
        auto& listeners = iter->second;

        (this->*pfnDispatchEventToListeners)(listeners, pEvent);
    }
}

void EventDispatcher::addEventListener(EventListener* pEventListener, Node* pTarget)
{
    OCF_ASSERT(pEventListener != nullptr);
    OCF_ASSERT(pTarget != nullptr);

    pEventListener->setAssociatedNode(pTarget);
    auto listenerId = pEventListener->getListenerId();
    m_listenerMap[listenerId].emplace_back(pEventListener);
}

void EventDispatcher::removeEventListener(EventListener* pEventListener)
{
    OCF_ASSERT(pEventListener != nullptr);

    auto listenerId = pEventListener->getListenerId();
    auto iter = m_listenerMap.find(listenerId);
    if (iter != m_listenerMap.end()) {
        auto& listeners = iter->second;
        auto listenerIter = std::find(listeners.begin(), listeners.end(), pEventListener);
        if (listenerIter != listeners.end()) {
            OCF_SAFE_RELEASE(*listenerIter);
            listeners.erase(listenerIter);
        }
    }
}

void EventDispatcher::removeEventListenerForTarget(Node* pTarget)
{
    for (auto& listenerVector : m_listenerMap) {

        auto& listeners = listenerVector.second;

        for (auto iter = listeners.begin(); iter != listeners.end();) {

            EventListener* listener = *iter;

            if (listener->getAssociatedNode() == pTarget) {
                listener->setAssociatedNode(nullptr);
                OCF_SAFE_RELEASE(listener);
                iter = listeners.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }
}

void EventDispatcher::dispatchEventToListeners(EventListenerVecotr& listeners, Event* event)
{
    for (auto& listener : listeners) {
        listener->m_onEvent(event);
    }
}

void EventDispatcher::dispatchMouseEventToListeners(EventListenerVecotr& listeners, Event* event)
{
    auto scene = Game::getInstance()->getCurrentScene();
    if (scene == nullptr) {
        return;
    }

    auto& cameras = scene->getCameras();
    for (auto& camera : cameras) {
        if (!camera->isVisible()) {
            continue;
        }

        Camera::s_pVisitingCamera = camera;

        for (size_t i = 0; i < listeners.size(); ++i) {
            auto& listener = listeners[i];
            if (listener->m_onEvent) {
                listener->m_onEvent(event);
            }
        }

        Camera::s_pVisitingCamera = nullptr;
    }
}

NS_OCF_END
