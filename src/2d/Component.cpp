#include "Component.h"

NS_OCF_BEGIN

Component::Component(Node* pEntity, int updateOrder)
    : m_pOwner(pEntity)
    , m_updateOrder(updateOrder)
{
}

Component::~Component()
{
}

void Component::update(float /* deltaTime */)
{
}

int Component::getUpdateOrder() const
{
    return m_updateOrder;
}

NS_OCF_END
