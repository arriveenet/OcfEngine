#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "base/Event.h"

NS_OCF_BEGIN

class EventListener;

class EventDispatcher : public GameObject {
public:
	class EventLisnnerVector {
	public:
		EventLisnnerVector();
		~EventLisnnerVector();

		std::vector<EventListener*>::iterator begin() noexcept
		{
			return m_eventListeners.begin();
		}

		std::vector<EventListener*>::iterator end() noexcept
		{
			return m_eventListeners.end();
		}

		void clear();
		std::vector<EventListener*>::iterator erase(std::vector<EventListener*>::iterator iterator);
		void emplace_back(EventListener* pEvent);

		std::vector<EventListener*> m_eventListeners;
	};

	EventDispatcher();
	~EventDispatcher();

	void dispatchEvent(Event* event);

	void addEventListener(EventListener* pEventListener, Node* pTarget);

	void removeEventLisnerForTarget(Node* pTarget);

private:
	std::unordered_map <std::string, EventLisnnerVector> m_listenerMap;
};

NS_OCF_END