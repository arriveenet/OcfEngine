#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "base/Event.h"

namespace ocf {

class EventListener;

using EventListenerVecotr = std::vector<EventListener*>;

class EventDispatcher : public GameObject {
public:
	EventDispatcher();
	~EventDispatcher();

	void dispatchEvent(Event* event);

	void addEventListener(EventListener* pEventListener, Node* pTarget);

	void removeEventLisnerForTarget(Node* pTarget);

private:
	std::unordered_map <std::string, EventListenerVecotr> m_listenerMap;
};

} // namespace ocf