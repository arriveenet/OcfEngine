#include "EventDispatcher.h"
#include "base/EventListener.h"

NS_OCF_BEGIN

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

void EventDispatcher::dispatchEvent(Event* event)
{
	for (auto&& listener : m_eventListeners) {
		if (listener->m_type == EventListener::Type::Keyboard) {
			listener->m_onEvent;
		}
	}
}

void EventDispatcher::addEventListener(EventListener* eventListener)
{
	m_eventListeners.push_back(eventListener);
}

NS_OCF_END
