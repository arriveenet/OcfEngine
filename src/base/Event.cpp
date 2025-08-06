#include "Event.h"
#include "base/Node.h"

NS_OCF_BEGIN

Event::Event(EventType eventType)
	: m_eventType(eventType)
	, m_pTarget(nullptr)
{
}

Event::~Event()
{
}

NS_OCF_END
