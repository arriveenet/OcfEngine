#pragma once
#include <vector>
#include "base/Event.h"

NS_OCF_BEGIN

class EventListener;

class EventDispatcher : public GameObject {
public:
	EventDispatcher();
	~EventDispatcher();

	void dispatchEvent(Event* event);

	void addEventListener(EventListener* eventListener);

private:
	std::vector<EventListener*> m_eventListeners;
};

NS_OCF_END