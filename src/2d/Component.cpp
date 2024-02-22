#include "Component.h"

OCF_BEGIN

Component::Component(Entity* pEntity, int updateOrder)
	: m_pOwner(pEntity)
	, m_updateOrder(updateOrder)
{
}

Component::~Component()
{
}

void Component::update(float deltaTime)
{
	m_updateOrder;
}

int Component::getUpdateOrder() const
{
	return 0;
}

OCF_END
