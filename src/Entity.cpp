#include "Entity.h"

Entity::Entity()
	: m_state(State::Active)
	, m_position()
	, m_size()
{
}

Entity::~Entity()
{
	while (!m_entities.empty())
	{
		delete m_entities.back();
		m_entities.pop_back();
	}

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
	for (const auto& entity : m_entities) {
		entity->update(deltaTime);
	}
}

void Entity::setPosition(const glm::vec2& position)
{
	m_position = position;
}

void Entity::setPosition(float x, float y)
{
	m_position = { x, y };
}

glm::vec2 Entity::getPosition() const
{
	return m_position;
}

void Entity::addChild(Entity* pEntity)
{
	m_entities.emplace_back(pEntity);
}

void Entity::removeChild(Entity* pEntity)
{
	auto iter = std::find(m_entities.begin(), m_entities.end(), pEntity);
	if (iter != m_entities.end()) {
		m_entities.erase(iter);
	}
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
