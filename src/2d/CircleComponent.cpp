#include "CircleComponent.h"
#include "2d/Entity.h"

OCF_BEGIN

CircleComponent::CircleComponent(Entity* pEntity)
	: Component(pEntity)
	, m_radius(0.0f)
{
}

float CircleComponent::getRadius() const
{
	return m_pOwner->getScale() * m_radius;
}

glm::vec2 CircleComponent::getCenter() const
{
	glm::vec2 center = m_pOwner->getPosition();
	return center;
}

bool intersectCircle(const CircleComponent& a, const CircleComponent& b)
{
	glm::vec2 diff = a.getCenter() - b.getCenter();
	float distSq = glm::length(diff);

	float radiiSq = a.getRadius() + b.getRadius();

	return (distSq <= radiiSq);
}

OCF_END
