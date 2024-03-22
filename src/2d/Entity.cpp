#include "Entity.h"
#include <glm/gtx/transform.hpp>

OCF_BEGIN

Entity::Entity()
	: m_state(State::Active)
	, m_position()
	, m_size()
	, m_rotation(0.0f)
	, m_scaleX(1.0f)
	, m_scaleY(1.0f)
	, m_scaleZ(1.0f)
	, m_transform(1.0f)
	, m_transformDirty(true)
{
}

Entity::~Entity()
{
	while (!m_entities.empty())
	{
		auto entity = m_entities.back();
		entity->release();
		m_entities.pop_back();
	}

	while (!m_components.empty()) {
		delete m_components.back();
	}
}

bool Entity::init()
{
	return true;
}

void Entity::processInput(const InputState& inputState)
{
	for (auto& entity : m_entities) {
		entity->processInput(inputState);
	}
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

void Entity::updateTransform()
{
	if (m_transformDirty) {
		m_transformDirty = false;

		m_transform = glm::translate(glm::vec3(m_position, 0));
		m_transform *= glm::rotate(glm::radians(m_rotation), glm::vec3(0, 0, 1));
		m_transform *= glm::scale(glm::vec3(m_scaleX, m_scaleY, m_scaleZ));
	}
}

void Entity::draw(Renderer* renderer, const glm::mat4& transform)
{
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

void Entity::setSize(float width, float height)
{
	m_size.x = width;
	m_size.y = height;
}

glm::vec2 Entity::getSize() const
{
	return m_size;
}

void Entity::setRotation(float rotation)
{
	m_rotation = rotation;
}

float Entity::getRotation() const
{
	return m_rotation;
}

void Entity::setScale(float scale)
{
	m_scaleX = m_scaleY = m_scaleZ = scale;
}

void Entity::setScaleX(float scaleX)
{
	m_scaleX = scaleX;
}

void Entity::setScaleY(float scaleY)
{
	m_scaleY = scaleY;
}

void Entity::setScaleZ(float scaleZ)
{
	m_scaleZ = scaleZ;
}

float Entity::getScale() const
{
	return m_scaleX;
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

OCF_END
