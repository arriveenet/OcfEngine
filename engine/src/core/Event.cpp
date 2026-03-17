#include "ocf/core/Event.h"
#include "ocf/core/Node.h"

namespace ocf {

Event::Event(EventType eventType)
	: m_eventType(eventType)
	, m_pTarget(nullptr)
{
}

Event::~Event()
{
}

} // namespace ocf
