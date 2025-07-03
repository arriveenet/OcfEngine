#include "MoveComponent.h"
#include <glm/glm.hpp>
#include "Node.h"

NS_OCF_BEGIN

MoveComponent::MoveComponent(Node* pEntity, int updateOrder)
    : Component(pEntity, updateOrder)
    , m_angularSpeed(0.0f)
    , m_forwardSpeed(0.0f)
{
}

void MoveComponent::update(float deltaTime)
{
    if (m_angularSpeed != 0.0f) {
        float rotation = m_pOwner->getRotation();
        rotation += m_angularSpeed * deltaTime;
        m_pOwner->setRotation(rotation);
    }

    const float angle = glm::radians(m_pOwner->getRotation());
    glm::vec2 foward = { glm::cos(angle), glm::sin(angle) };
    if (m_forwardSpeed != 0.0f) {
        glm::vec2 position = m_pOwner->getPosition();
        position += foward * m_forwardSpeed * deltaTime;
        m_pOwner->setPosition(position);
    }
}


NS_OCF_END

