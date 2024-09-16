#pragma once
#include "base/GameObject.h"

NS_OCF_BEGIN

class Node;

enum class EventType {
	None,
	KeyEvent,
	MouseEvent,
};

class Event : public GameObject {
public:
	Event(Node* target, EventType eventType);
	virtual ~Event();

	Node* getTarget() const { return m_pTarget; }

	EventType getEventType() const { return m_eventType; }

protected:
	EventType m_eventType;
	Node* m_pTarget;
};

NS_OCF_END
