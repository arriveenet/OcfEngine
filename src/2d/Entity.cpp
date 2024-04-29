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
	while (!m_entities.empty()) {
		auto entity = m_entities.back();
		entity->release();
		m_entities.pop_back();
	}

	while (!m_components.empty()) {
		auto component = m_components.back();
		delete component;
		m_components.pop_back();
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
	// コンポーネントを更新
	updateComponents(deltaTime);
	// 配下のエンティティを更新
	updateEntitices(deltaTime);

	//　自身を更新
	updateEntity(deltaTime);
}

void Entity::updateEntity(float deltaTime)
{
}

void Entity::updateComponents(float deltaTime)
{
	for (const auto& comp : m_components) {
		comp->update(deltaTime);
	}
}

void Entity::updateEntitices(float deltaTime)
{
	for (const auto& entity : m_entities) {
		entity->update(deltaTime);
	}

	std::vector<Entity*> deadEntities;
	for (const auto& entity : m_entities) {
		if (entity->getState() == Entity::Dead) {
			deadEntities.emplace_back(entity);
		}
	}

	for (auto entity : deadEntities) {
		removeChild(entity);
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
	m_transformDirty = true;
}

void Entity::setPosition(float x, float y)
{
	m_position = { x, y };
	m_transformDirty = true;
}

glm::vec2 Entity::getPosition() const
{
	return m_position;
}

void Entity::setSize(float width, float height)
{
	m_size.x = width;
	m_size.y = height;
	m_transformDirty = true;
}

glm::vec2 Entity::getSize() const
{
	return m_size;
}

void Entity::setRotation(float rotation)
{
	m_rotation = rotation;
	m_transformDirty = true;
}

float Entity::getRotation() const
{
	return m_rotation;
}

void Entity::setScale(float scale)
{
	m_scaleX = m_scaleY = m_scaleZ = scale;
	m_transformDirty = true;
}

void Entity::setScaleX(float scaleX)
{
	m_scaleX = scaleX;
	m_transformDirty = true;
}

void Entity::setScaleY(float scaleY)
{
	m_scaleY = scaleY;
	m_transformDirty = true;
}

void Entity::setScaleZ(float scaleZ)
{
	m_scaleZ = scaleZ;
	m_transformDirty = true;
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
		delete pEntity;
		pEntity = nullptr;
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

void Entity::visit(Renderer* pRenderer, const glm::mat4& parentTransform)
{
	if (!m_entities.empty()) {
		this->draw(pRenderer, parentTransform);

		for (auto iter = m_entities.cbegin(); iter != m_entities.cend(); ++iter) {
			(*iter)->visit(pRenderer, parentTransform);
		}
	}
	else {
		this->draw(pRenderer, parentTransform);
	}
}

OCF_END
