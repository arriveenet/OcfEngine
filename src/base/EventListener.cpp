#include "EventListener.h"

NS_OCF_BEGIN

EventListener::EventListener()
{
}

EventListener::~EventListener()
{
}

bool EventListener::init(Type type, const std::function<void(Event*)>& callback)
{
	m_type = type;
	m_onEvent = callback;

	return true;
}

EventListenerKeyboard* EventListenerKeyboard::create()
{
	EventListenerKeyboard* listener = new EventListenerKeyboard();
	if (listener->init()) {
		listener->autorelease();
		return listener;
	}
	OCF_SAFE_DELETE(listener);
	return nullptr;
}

bool EventListenerKeyboard::init()
{
	auto listener = [this](Event* event) {
		
		};

	if (EventListener::init(Type::Keyboard, listener)) {
		return true;
	}

	return false;
}

NS_OCF_END
