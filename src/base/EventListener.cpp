#include "EventListener.h"

NS_OCF_BEGIN

EventListener::EventListener()
	: m_pNode(nullptr)
    , m_type(Type::Unknown)
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

NS_OCF_END
