#include "Entity.h"
#include <glm/gtx/transform.hpp>
#include "base/Game.h"

OCF_BEGIN

Entity::Entity()
	: m_state(State::Active)
	, m_pParent(nullptr)
	, m_position()
	, m_size()
	, m_rotation(0.0f)
	, m_scaleX(1.0f)
	, m_scaleY(1.0f)
	, m_scaleZ(1.0f)
	, m_transform(1.0f)
	, m_modelVewTransform(1.0f)
	, m_transformDirty(true)
	, m_transformUpdated(true)
	, m_contentSizeDirty(true)
{
	m_pGame = Game::getInstance();
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
	// �R���|�[�l���g���X�V
	updateComponents(deltaTime);
	// �z���̃G���e�B�e�B���X�V
	updateEntitices(deltaTime);

	//�@���g���X�V
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

	pEntity->setParent(this);
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

size_t Entity::getChildCount() const
{
	return m_entities.size();
}

void Entity::setParent(Entity* pEntity)
{
	m_pParent = pEntity;
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

const glm::mat4& Entity::getEntityToParentTransform()
{
	if (m_transformDirty) {
		glm::mat4 transform = glm::translate(glm::vec3(m_position, 0));
		transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(m_scaleX, m_scaleY, m_scaleZ));

		m_transform = transform;
	}

	m_transformDirty = false;

	return m_transform;
}

void Entity::visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags)
{
	uint32_t flags = processParentFlag(parentTransform, parentFlags);

	m_pGame->pushMatrix(MatrixStack::ModelView);
	m_pGame->loadMatrix(MatrixStack::ModelView, m_modelVewTransform);

	if (!m_entities.empty()) {
		// ���g��`��
		this->draw(pRenderer, m_modelVewTransform);

		// �q�G���e�B�e�B��`��
		for (auto iter = m_entities.cbegin(); iter != m_entities.cend(); ++iter) {
			(*iter)->visit(pRenderer, m_modelVewTransform, flags);
		}
	}
	else {
		this->draw(pRenderer, m_modelVewTransform);
	}

	m_pGame->popMatrix(MatrixStack::ModelView);
}

glm::mat4 Entity::transform(const glm::mat4& parentTransform)
{
	return parentTransform * this->getEntityToParentTransform();
}

uint32_t Entity::processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag)
{
	uint32_t flags = parentFlag;
	flags |= (m_transformUpdated ? FLAGS_TRANSFORM_DIRTY : 0);
	flags |= (m_contentSizeDirty ? FLAGS_CONTENT_SIZE_DIRTY : 0);

	if (flags & FLAGS_TRANSFORM_DIRTY) {
		m_modelVewTransform = this->transform(parentTransform);
	}

	m_transformUpdated = false;
	m_contentSizeDirty = false;

	return flags;
}

OCF_END
