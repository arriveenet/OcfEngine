#include "Component.h"

namespace ocf {

Component::Component(Node2D* pEntity, int updateOrder)
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

} // namespace ocf
