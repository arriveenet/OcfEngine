#pragma once
#include "base/GameObject.h"

NS_OCF_BEGIN

class Node;

enum class EventType {
	KeyboardEvent,
	MouseEvent,
};

class Event : public GameObject {
public:
	Event(EventType eventType);
	virtual ~Event();

	void setTarget(Node* pTarget) { m_pTarget = pTarget; }
	Node* getTarget() const { return m_pTarget; }

	EventType getEventType() const { return m_eventType; }

protected:
	EventType m_eventType;
	Node* m_pTarget;
};

NS_OCF_END
