#include "Node.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "base/Game.h"
#include "base/Rect.h"
#include "base/EventDispatcher.h"
#include "2d/Camera.h"
#include "2d/Scene.h"

NS_OCF_BEGIN

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
    , m_localZOrder(0)
    , m_globalZOrder(0.0f)
    , m_visible(true)
    , m_ignoreAnchorPointForPosition(false)
    , m_transformDirty(true)
    , m_transformUpdated(true)
    , m_contentSizeDirty(true)
    , m_name("Node")
{
    m_pGame = Game::getInstance();
    m_pEventDispatcher = Game::getInstance()->getEventDispatcher();
}

Node::~Node()
{
    m_pEventDispatcher->removeEventLisnerForTarget(this);

    while (!m_children.empty()) {
        auto entity = m_children.back();
        entity->onExit();
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

void Node::update(float deltaTime)
{
    // コンポーネントを更新
    updateComponents(deltaTime);
    // 配下のエンティティを更新
    updateNodes(deltaTime);

    //　自身を更新
    updateNode(deltaTime);
}

void Node::updateNode(float /*deltaTime*/)
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

void Node::onEnter()
{
    for (const auto& child : m_children) {
        child->onEnter();
    }
}

void Node::onExit()
{
    for (const auto& child : m_children) {
        child->onExit();
    }
}

void Node::draw(Renderer* /*renderer*/, const glm::mat4& /*transform*/)
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

bool Node::hitTest(const glm::vec2& worldPoint) const
{
    glm::vec2 point = this->convertToNodeSpace(worldPoint);
    glm::vec2 size = this->getSize();
    return Rect(0.0f, 0.0f, size.x, size.y).intersect(point);
}

void Node::setVisible(bool visible)
{
    if (visible != m_visible) {
        m_visible = visible;
        if (m_visible)
            m_transformUpdated = m_transformDirty = true;
    }
}

bool Node::isVisible() const
{
    return m_visible;
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

float Node::getScaleX() const
{
    return m_scaleX;
}

float Node::getScaleY() const
{
    return m_scaleY;
}

float Node::getScaleZ() const
{
    return m_scaleZ;
}

glm::vec3 Node::getScale() const
{
    return glm::vec3(m_scaleX, m_scaleY, m_scaleZ);
}

void Node::setGlobalZOrder(float globalZorder)
{
    if (m_globalZOrder != globalZorder) {
        m_globalZOrder = globalZorder;
    }
}

std::string Node::getName() const
{
    return m_name;
}

void Node::setName(const std::string& name)
{
    m_name = name;
}

void Node::addChild(Node* pNode)
{
    OCFASSERT(pNode != nullptr, "The pointer to the node to be added is NULL.");

    m_children.emplace_back(pNode);

    pNode->setParent(this);

    pNode->onEnter();
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

void Node::sortAllChildren()
{
    sortNodes(m_children);
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

glm::mat4 Node::getWorldToNodeTransform() const
{
    return glm::inverse(getNodeToWorldTransform());
}

glm::vec2 Node::convertToNodeSpace(const glm::vec2& worldPoint) const
{
    glm::mat4 transform = getWorldToNodeTransform();
    glm::vec4 point(worldPoint, 0.0f, 1.0f);
    glm::vec2 result = transform * point;
    return result;
}

glm::vec2 Node::convertToWorldSpace(const glm::vec2& nodePoint) const
{
    glm::mat4 transform = getNodeToWorldTransform();
    glm::vec4 point(nodePoint, 0.0f, 1.0f);
    glm::vec2 result = transform * point;
    return result;
}

void Node::visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags)
{
    if (!m_visible) {
        return;
    }

    uint32_t flags = processParentFlag(parentTransform, parentFlags);

    m_pGame->pushMatrix(MatrixStack::ModelView);
    m_pGame->loadMatrix(MatrixStack::ModelView, m_modelVewTransform);

    const bool visibleByCamera = isVisitableByVisitingCamera();

    if (!m_children.empty()) {
        sortAllChildren();

        for (auto iter = m_children.cbegin(); iter != m_children.cend(); ++iter) {
            if ((*iter)->m_localZOrder < 0) {
                (*iter)->visit(pRenderer, m_modelVewTransform, flags);
            }
            else {
                break;
            }
        }

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

Scene* Node::getScene() const
{
    if(m_pParent == nullptr)
        return nullptr;

    auto* sceneNode = m_pParent;
    while (sceneNode->m_pParent != nullptr) {
        sceneNode = sceneNode->m_pParent;
    }

    return dynamic_cast<Scene*>(sceneNode);
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

bool isScreenPointInRect(const glm::vec2& pt, const Camera* pCamera, const glm::mat4& worldToLocal,
                         const Rect& rect, glm::vec3* p)
{
    if (pCamera == nullptr || rect.m_size.x <= 0 || rect.m_size.y <= 0) {
        return false;
    }

    glm::vec3 Pn(pt.x, pt.y, -1.0f), Pf(pt.x, pt.y, 1.0f);
    Pn = pCamera->unProjectGL(Pn);
    Pf = pCamera->unProjectGL(Pf);

    Pn = worldToLocal * glm::vec4(Pn, 1.0f);
    Pf = worldToLocal * glm::vec4(Pf, 1.0f);

    glm::vec3 E = Pf - Pn;

    glm::vec3 A = glm::vec3(rect.m_position.x, rect.m_position.y, 0);
    glm::vec3 B = glm::vec3(rect.m_position.x + rect.m_size.x, rect.m_position.y, 0);
    glm::vec3 C = glm::vec3(rect.m_position.x, rect.m_position.y + rect.m_size.y, 0);

    B = B - A;
    C = C - A;

    glm::vec3 BxC = glm::cross(B, C);
    float BxCdotE = glm::dot(BxC, E);
    if (BxCdotE == 0)
    {
        return false;
    }
    float t = (glm::dot(BxC, A) - glm::dot(BxC, Pn)) / BxCdotE;
    glm::vec3 P = Pn + t * E;
    if (p) {
        *p = P;
    }

    return rect.intersect(glm::vec2(P.x, P.y));
}

NS_OCF_END
