#pragma once
#include <cstdint>
#include <algorithm>
#include <vector>
#include <glm/glm.hpp>
#include "2d/Component.h"
#include "base/GameObject.h"
#include "input/Input.h"

NS_OCF_BEGIN

class Game;
class EventDispatcher;
class Renderer;
class Scene;
class Rect;
class Camera2D;

/**
 * @brief ノードクラス
 */
class Node : public GameObject {
public:
    enum {
        FLAGS_TRANSFORM_DIRTY		= (1 << 0),
        FLAGS_CONTENT_SIZE_DIRTY	= (1 << 1),
        FLAGS_RENDER_AS_3D			= (1 << 3),

        FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
    };
    /** デフォルトコンストラクタ */
    Node();
    /** デストラクタ */
    virtual ~Node();

    virtual bool init();

    virtual void update(float deltaTime);
    virtual void updateNode(float deltaTime);
    virtual void updateComponents(float deltaTime);
    virtual void updateNodes(float deltaTime);

    virtual void onEnter();
    virtual void onExit();

    virtual void draw(Renderer* renderer, const glm::mat4& transform);

    virtual void setGlobalZOrder(float globalZorder);
    virtual float getGlobalZOrder() const { return m_globalZOrder; }

    virtual void setLocalZOrder(std::int32_t localZOrder);
    virtual std::int32_t getLocalZOrder() const { return m_localZOrder; }

    std::string getName() const;
    void setName(const std::string& name);

    /**
     * @brief 子ノードを追加
     * @param pNode 追加する子ノード
     */
    virtual void addChild(Node* pNode);
    /**
     * @brief 子ノードを削除
     * @param pNode 削除する子ノード
     */
    virtual void removeChild(Node* pNode);
    /**
     * @brief 子ノードの数を取得
     * @return 子ノードの数
     */
    virtual size_t getChildCount() const;
    /**
     * @brief 親ノードを設定
     * @param pNode 設定する親ノード
     */
    virtual void setParent(Node* pNode);
    /**
     * @brief 親ノードを取得
     * @return pNode 親ノード
     */
    virtual Node* getParent() { return m_pParent; }

    /**
     * @brief 子ノードをすべてソートする
     */
    virtual void sortAllChildren();

    /**
     * @brief ソートのヘルパ関数
     * @param nodes ノードの配列
     */
    template<typename T>
    inline static void sortNodes(std::vector<T*>& nodes)
    {
        static_assert(std::is_base_of<Node, T>::value, "Node::sortNodes: Only accept derived of Node!");
        std::sort(std::begin(nodes), std::end(nodes),
            [](T* n1, T* n2) { return (n1->m_localZOrder < n2->m_localZOrder); });
    }

    /**
     * @brief カメラのマスクを取得
     * @return カメラのマスク
     */
    uint16_t getCameraMask() const { return m_cameraMask; }
    /**
     * @brief カメラのマスクを設定
     * @param uint16_t カメラのマスク
     * @param applyChildren 子ノードにもカメラマスクを設定するかどうか
     */
    virtual void setCameraMask(uint16_t mask, bool applyChildren = true);

    void addComponent(Component* pComponent);
    void removeComponent(Component* pComponent);

    virtual void visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags);

    virtual Scene* getScene() const;

protected:
    bool isVisitableByVisitingCamera() const;
    
protected:
    Node* m_pParent;                        //!< 親ノード
    Game* m_pGame;                          //!< シングルトンのゲームクラス
    EventDispatcher* m_pEventDispatcher;    // イベントディスパッチ

    uint16_t m_cameraMask;                  //!< カメラマスク

    std::int32_t m_localZOrder;             //!< ローカルZオーダー
    float m_globalZOrder;                   //!< グローバルZオーダー

    std::vector<Node*> m_children;          //!< 子ノードの配列
    std::vector<Component*> m_components;   //!< コンポーネント
    std::string m_name;                     //!< ノードの名前
};

bool isScreenPointInRect(const glm::vec2& pt, const Camera2D* pCamera, const glm::mat4& worldToLocal,
                         const Rect& rect, glm::vec3* p);

NS_OCF_END
