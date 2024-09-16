#include "Event.h"
#include "2d/Node.h"

NS_OCF_BEGIN

Event::Event(Node* target, EventType eventType)
	: m_pTarget(target)
	, m_eventType(eventType)
{
}

Event::~Event()
{
}

NS_OCF_END
