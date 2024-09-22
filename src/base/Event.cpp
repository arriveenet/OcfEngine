#include "Event.h"
#include "2d/Node.h"

NS_OCF_BEGIN

Event::Event(EventType eventType)
	: m_pTarget(nullptr)
	, m_eventType(eventType)
{
}

Event::~Event()
{
}

NS_OCF_END
