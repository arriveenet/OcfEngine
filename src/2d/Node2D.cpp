#include "Node2d.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "base/Game.h"
#include "base/Rect.h"

NS_OCF_BEGIN

Node2D::Node2D()
    : m_position(0.0f, 0.0f)
    , m_size(0.0f, 0.0f)
    , m_rotation(0.0f)
    , m_scale(1.0f, 1.0f)
    , m_anchorPointInPoints()
    , m_anchorPoint()
    , m_transform(1.0f)
    , m_modelVewTransform(1.0f)
    , m_ignoreAnchorPointForPosition(false)
    , m_transformDirty(true)
    , m_transformUpdated(true)
    , m_contentSizeDirty(true)
{
}

Node2D::~Node2D()
{
}

void Node2D::setPosition(const glm::vec2& position)
{
    m_position.x = position.x;
    m_position.y = position.y;
    m_transformUpdated = m_transformDirty = true;
}

void Node2D::setSize(const glm::vec2& size)
{
    if (size != m_size) {
        m_size = size;
        m_anchorPointInPoints = {m_size.x * m_anchorPoint.x, m_size.y * m_anchorPoint.y};
        m_transformUpdated = m_transformDirty = true;
    }
}

void Node2D::setRotation(float rotation)
{
    m_rotation = rotation;
    m_transformUpdated = m_transformDirty = true;
}

void Node2D::setScale(const glm::vec2& scale)
{
    m_scale = scale;
    m_transformUpdated = m_transformDirty = true;
}


glm::vec2 Node2D::getPosition() const
{
    return m_position;
}

glm::vec2 Node2D::getSize() const
{
    return m_size;
}

glm::vec2 Node2D::getScale() const
{
    return m_scale;
}

float Node2D::getRotation() const
{
    return m_rotation;
}

void Node2D::setAnchorPoint(const glm::vec2& point)
{
    if (point != m_anchorPoint) {
        m_anchorPoint = point;
        m_anchorPointInPoints = {m_size.x * m_anchorPoint.x, m_size.y * m_anchorPoint.y};
        m_transformUpdated = m_transformDirty = true;
    }
}

const glm::vec2& Node2D::getAnchorPoint() const
{
    return m_anchorPoint;
}

bool Node2D::hitTest(const glm::vec2& worldPoint) const
{
    glm::vec2 point = this->convertToNodeSpace(worldPoint);
    glm::vec2 size = this->getSize();
    return Rect(0.0f, 0.0f, size.x, size.y).intersect(point);
}

const glm::mat4& Node2D::getNodeToParentTransform() const
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
        transform = glm::scale(transform, glm::vec3(m_scale.x, m_scale.y, 1.0f));

        m_transform = transform;

        // 基準点を調整
        if (m_anchorPointInPoints != glm::vec2(0.0f, 0.0f)) {
            m_transform[3][0] += m_transform[0][0] * -m_anchorPointInPoints.x +
                                 m_transform[1][0] * -m_anchorPointInPoints.y;
            m_transform[3][1] += m_transform[0][1] * -m_anchorPointInPoints.x +
                                 m_transform[1][1] * -m_anchorPointInPoints.y;
            m_transform[3][2] += m_transform[0][2] * -m_anchorPointInPoints.x +
                                 m_transform[1][2] * -m_anchorPointInPoints.y;
        }
    }

    m_transformDirty = false;

    return m_transform;
}

glm::mat4 Node2D::getNodeToParentTransform(Node* ancestor) const
{
    glm::mat4 t(this->getNodeToParentTransform());

    for (Node* p = m_pParent; p != nullptr && p != ancestor; p = p->getParent()) {
        t = static_cast<Node2D*>(p)->getNodeToParentTransform() * t;
    }

    return t;
}

glm::mat4 Node2D::getNodeToWorldTransform() const
{
    return getNodeToParentTransform(nullptr);
}

glm::mat4 Node2D::getWorldToNodeTransform() const
{
    return glm::inverse(getNodeToWorldTransform());
}

glm::vec2 Node2D::convertToNodeSpace(const glm::vec2& worldPoint) const
{
    glm::mat4 transform = getWorldToNodeTransform();
    glm::vec4 point(worldPoint, 0.0f, 1.0f);
    glm::vec2 result = transform * point;
    return result;
}

glm::vec2 Node2D::convertToWorldSpace(const glm::vec2& nodePoint) const
{
    glm::mat4 transform = getNodeToWorldTransform();
    glm::vec4 point(nodePoint, 0.0f, 1.0f);
    glm::vec2 result = transform * point;
    return result;
}

void Node2D::visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags)
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
            if ((*iter)->getLocalZOrder() < 0) {
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

glm::mat4 Node2D::transform(const glm::mat4& parentTransform)
{
    return parentTransform * this->getNodeToParentTransform();
}

uint32_t Node2D::processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag)
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


NS_OCF_END
