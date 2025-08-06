#include "Node.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "2d/Camera2D.h"
#include "base/Game.h"
#include "base/Rect.h"
#include "base/EventDispatcher.h"
#include "base/Scene.h"
#include "base/Viewport.h"
#include <typeinfo>

NS_OCF_BEGIN

Node::Node()
    : m_pParent(nullptr)
    , m_cameraMask(1)
    , m_localZOrder(0)
    , m_globalZOrder(0.0f)
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

void Node::setGlobalZOrder(float globalZorder)
{
    if (m_globalZOrder != globalZorder) {
        m_globalZOrder = globalZorder;
    }
}

void Node::setLocalZOrder(std::int32_t localZOrder)
{
    if (m_localZOrder != localZOrder) {
        m_localZOrder = localZOrder;
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

void Node::visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags)
{
    const bool visibleByCamera = isVisitableByVisitingCamera();

    if (!m_children.empty()) {
        sortAllChildren();

        for (auto iter = m_children.cbegin(); iter != m_children.cend(); ++iter) {
            if ((*iter)->getLocalZOrder() < 0) {
                (*iter)->visit(pRenderer, parentTransform, parentFlags);
            }
            else {
                break;
            }
        }

        // 自身を描画
        if (visibleByCamera) {
            this->draw(pRenderer, parentTransform);
        }

        // 子エンティティを描画
        for (auto iter = m_children.cbegin(); iter != m_children.cend(); ++iter) {
            (*iter)->visit(pRenderer, parentTransform, parentFlags);
        }
    }
    else if (visibleByCamera) {
        this->draw(pRenderer, parentTransform);
    }
    else {
        // 何もしない
    }
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
    Camera2D* pCamera = Camera2D::getVisitingCamera();
    bool visibleByCamera = (pCamera != nullptr) ? static_cast<uint16_t>(pCamera->getCameraFlag()) & m_cameraMask : true;
    return visibleByCamera;
}

void Node::setScene(Scene* scene)
{
    if (m_scene != nullptr) {
        propagateExitTree();
    }

    m_scene = scene;

    if (m_scene != nullptr) {
        propagateEnterTree();
    }
}

void Node::propagateEnterTree()
{
    if (m_pParent) {
        m_scene = m_pParent->m_scene;
        m_depth = m_pParent->m_depth + 1;
    }
    else {
        m_depth = 1;
    }

    m_viewport = dynamic_cast<Viewport*>(this);
    if (!m_viewport && m_pParent) {
        m_viewport = m_pParent->m_viewport;
    }

    m_scene->addNode(this);

    for (const auto& child : m_children) {
        child->propagateEnterTree();
    }
}

void Node::propagateExitTree()
{
}

bool isScreenPointInRect(const glm::vec2& pt, const Camera2D* pCamera, const glm::mat4& worldToLocal,
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
