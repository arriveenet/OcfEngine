#include "Entity.h"

Entity::Entity()
	: m_state(State::Active)
	, m_position()
{
}

Entity::~Entity()
{
	while (!m_components.empty()) {
		delete m_components.back();
	}
}

bool Entity::init()
{
	return false;
}

void Entity::update(float deltaTime)
{
	updateComponents(deltaTime);
	updateEntity(deltaTime);
}

void Entity::updateComponents(float deltaTime)
{
	for (const auto& comp : m_components) {
		comp->update(deltaTime);
	}
}

void Entity::updateEntity(float deltaTime)
{
}

void Entity::setPosition(const glm::vec2& position)
{
	m_position = position;
}

glm::vec2 Entity::getPosition() const
{
	return m_position;
}

void Entity::addComponent(Component* pComponent)
{
	int myOrder = pComponent->getUpdateOrder();
	auto iter = m_components.begin();
	for (; iter != m_components.end(); ++iter) {
		if (myOrder < (*iter)->getUpdateOrder()) {
			break;
		}
	}

	m_components.insert(iter, pComponent);
}

void Entity::removeComponent(Component* pComponent)
{
	auto iter = std::find(m_components.begin(), m_components.end(), pComponent);
	if (iter != m_components.end()) {
		m_components.erase(iter);
	}
}
