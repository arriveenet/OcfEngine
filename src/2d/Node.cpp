#include "Node.h"
#include <glm/gtx/transform.hpp>
#include "base/Game.h"
#include "2d/Camera.h"

OCF_BEGIN

Node::Node()
	: m_pParent(nullptr)
	, m_cameraMask(1)
	, m_position()
	, m_size()
	, m_anchorPointInPoints()
	, m_anchorPoint()
	, m_rotation(0.0f)
	, m_scaleX(1.0f)
	, m_scaleY(1.0f)
	, m_scaleZ(1.0f)
	, m_transform(1.0f)
	, m_modelVewTransform(1.0f)
	, m_ignoreAnchorPointForPosition(false)
	, m_transformDirty(true)
	, m_transformUpdated(true)
	, m_contentSizeDirty(true)
{
	m_pGame = Game::getInstance();
}

Node::~Node()
{
	while (!m_children.empty()) {
		auto entity = m_children.back();
		entity->release();
		m_children.pop_back();
	}

	while (!m_components.empty()) {
		auto component = m_components.back();
		delete component;
		m_components.pop_back();
	}
}

bool Node::init()
{
	return true;
}

void Node::processInput(const InputState& inputState)
{
	for (auto& entity : m_children) {
		entity->processInput(inputState);
	}
}

void Node::update(float deltaTime)
{
	// コンポーネントを更新
	updateComponents(deltaTime);
	// 配下のエンティティを更新
	updateNodes(deltaTime);

	//　自身を更新
	updateNode(deltaTime);
}

void Node::updateNode(float deltaTime)
{
}

void Node::updateComponents(float deltaTime)
{
	for (const auto& comp : m_components) {
		comp->update(deltaTime);
	}
}

void Node::updateNodes(float deltaTime)
{
	for (const auto& child : m_children) {
		child->update(deltaTime);
	}
}

void Node::updateTransform()
{
	if (m_transformDirty) {
		m_transformDirty = false;

		m_transform = glm::translate(m_position);
		m_transform *= glm::rotate(glm::radians(m_rotation), glm::vec3(0, 0, 1));
		m_transform *= glm::scale(glm::vec3(m_scaleX, m_scaleY, m_scaleZ));
	}
}

void Node::draw(Renderer* renderer, const glm::mat4& transform)
{
}

void Node::setPosition(const glm::vec3& position)
{
	if (m_position != position) {
		m_position = position;
		m_transformUpdated = m_transformDirty = true;
	}
}

void Node::setPosition(const glm::vec2& position)
{
	m_position.x = position.x;
	m_position.y = position.y;
	m_transformUpdated = m_transformDirty = true;
}

void Node::setPosition(float x, float y, float z)
{
	setPosition(glm::vec3(x, y, z));
}

void Node::setPosition(float x, float y)
{
	setPosition(glm::vec2(x, y));
}

glm::vec3 Node::getPosition() const
{
	return m_position;
}

void Node::setSize(const glm::vec2& size)
{
	if (size != m_size) {
		m_size = size;
		m_anchorPointInPoints = { m_size.x * m_anchorPoint.x, m_size.y * m_anchorPoint.y };
		m_transformUpdated = m_transformDirty = true;
	}
}

void Node::setSize(float width, float height)
{
	setSize(glm::vec2(width, height));
}

glm::vec2 Node::getSize() const
{
	return m_size;
}

void Node::setAnchorPoint(const glm::vec2& point)
{
	if (point != m_anchorPoint) {
		m_anchorPoint = point;
		m_anchorPointInPoints = {m_size.x * m_anchorPoint.x, m_size.y * m_anchorPoint.y};
		m_transformUpdated = m_transformDirty = true;
	}
}

const glm::vec2& Node::getAnchorPoint() const
{
	return m_anchorPoint;
}

void Node::setRotation(float rotation)
{
	m_rotation = rotation;
	m_transformUpdated = m_transformDirty = true;
}

float Node::getRotation() const
{
	return m_rotation;
}

void Node::setScale(float scale)
{
	m_scaleX = m_scaleY = m_scaleZ = scale;
	m_transformUpdated = m_transformDirty = true;
}

void Node::setScaleX(float scaleX)
{
	m_scaleX = scaleX;
	m_transformUpdated = m_transformDirty = true;
}

void Node::setScaleY(float scaleY)
{
	m_scaleY = scaleY;
	m_transformUpdated = m_transformDirty = true;
}

void Node::setScaleZ(float scaleZ)
{
	m_scaleZ = scaleZ;
	m_transformUpdated = m_transformDirty = true;
}

float Node::getScale() const
{
	return m_scaleX;
}

void Node::addChild(Node* pEntity)
{
	m_children.emplace_back(pEntity);

	pEntity->setParent(this);
}

void Node::removeChild(Node* pEntity)
{
	auto iter = std::find(m_children.begin(), m_children.end(), pEntity);
	if (iter != m_children.end()) {
		m_children.erase(iter);
		delete pEntity;
		pEntity = nullptr;
	}
}

size_t Node::getChildCount() const
{
	return m_children.size();
}

void Node::setParent(Node* pEntity)
{
	m_pParent = pEntity;
}

void Node::setCameraMask(uint16_t mask, bool applyChildren)
{
	m_cameraMask = mask;

	if (applyChildren) {
		for (const auto& child : m_children) {
			child->setCameraMask(mask, applyChildren);
		}
	}
}

void Node::addComponent(Component* pComponent)
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

void Node::removeComponent(Component* pComponent)
{
	auto iter = std::find(m_components.begin(), m_components.end(), pComponent);
	if (iter != m_components.end()) {
		m_components.erase(iter);
	}
}

const glm::mat4& Node::getNodeToParentTransform() const
{
	if (m_transformDirty) {
		float x = m_position.x;
		float y = m_position.y;
		float z = 0.0f;

		if (m_ignoreAnchorPointForPosition) {
			x += m_anchorPointInPoints.x;
			y += m_anchorPointInPoints.y;
		}

		glm::mat4 transform = glm::translate(glm::vec3(x, y, z));
		transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(m_scaleX, m_scaleY, m_scaleZ));

		m_transform = transform;

		// 基準点を調整
		if (m_anchorPointInPoints != glm::vec2(0.0f, 0.0f)) {
			m_transform[3][0] += m_transform[0][0] * -m_anchorPointInPoints.x + m_transform[1][0] * -m_anchorPointInPoints.y;
			m_transform[3][1] += m_transform[0][1] * -m_anchorPointInPoints.x + m_transform[1][1] * -m_anchorPointInPoints.y;
			m_transform[3][2] += m_transform[0][2] * -m_anchorPointInPoints.x + m_transform[1][2] * -m_anchorPointInPoints.y;
		}
	}

	m_transformDirty = false;

	return m_transform;
}

glm::mat4 Node::getNodeToParentTransform(Node* ancestor) const
{
	glm::mat4 t(this->getNodeToParentTransform());

	for (Node* p = m_pParent; p != nullptr && p != ancestor; p = p->getParent()) {
		t = p->getNodeToParentTransform() * t;
	}

	return t;
}

glm::mat4 Node::getNodeToWorldTransform() const
{
	return getNodeToParentTransform(nullptr);
}

void Node::visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags)
{
	uint32_t flags = processParentFlag(parentTransform, parentFlags);

	m_pGame->pushMatrix(MatrixStack::ModelView);
	m_pGame->loadMatrix(MatrixStack::ModelView, m_modelVewTransform);

	const bool visibleByCamera = isVisitableByVisitingCamera();

	if (!m_children.empty()) {
		// 自身を描画
		if (visibleByCamera) {
			this->draw(pRenderer, m_modelVewTransform);
		}

		// 子エンティティを描画
		for (auto iter = m_children.cbegin(); iter != m_children.cend(); ++iter) {
			(*iter)->visit(pRenderer, m_modelVewTransform, flags);
		}
	}
	else if (visibleByCamera) {
		this->draw(pRenderer, m_modelVewTransform);
	}
	else {
		// 何もしない
	}

	m_pGame->popMatrix(MatrixStack::ModelView);
}

bool Node::isVisitableByVisitingCamera() const
{
	Camera* pCamera = Camera::getVisitingCamera();
	bool visibleByCamera = (pCamera != nullptr) ? static_cast<uint16_t>(pCamera->getCameraFlag()) & m_cameraMask : true;
	return visibleByCamera;
}

glm::mat4 Node::transform(const glm::mat4& parentTransform)
{
	return parentTransform * this->getNodeToParentTransform();
}

uint32_t Node::processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag)
{
	if (!isVisitableByVisitingCamera()) {
		return parentFlag;
	}

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
