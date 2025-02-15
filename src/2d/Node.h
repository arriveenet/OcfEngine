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
class Camera;

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
    virtual void updateTransform();

    virtual void onEnter();
    virtual void onExit();

    virtual void draw(Renderer* renderer, const glm::mat4& transform);

    virtual void setPosition(const glm::vec3& position);
    virtual void setPosition(const glm::vec2& position);
    virtual void setPosition(float x, float y, float z);
    virtual void setPosition(float x, float y);
    virtual glm::vec3 getPosition() const;

    virtual void setSize(const glm::vec2& size);
    virtual void setSize(float width, float height);
    virtual glm::vec2 getSize() const;

    virtual bool hitTest(const glm::vec2& worldPoint) const;

    virtual void setVisible(bool visible);
    virtual bool isVisible() const;

    virtual void setAnchorPoint(const glm::vec2& point);
    virtual const glm::vec2& getAnchorPoint() const;

    virtual void setRotation(float rotation);
    virtual float getRotation() const;

    virtual void setScale(float scale);
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScaleZ(float scaleZ);
    virtual float getScaleX() const;
    virtual float getScaleY() const;
    virtual float getScaleZ() const;
    virtual glm::vec3 getScale() const;

    virtual void setGlobalZOrder(float globalZorder);
    virtual float getGlobalZOrder() const { return m_globalZOrder; }

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
            [](T* n1, T* n2) { return (n1->m_localZOrder < n1->m_localZOrder); });
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

    virtual const glm::mat4& getNodeToParentTransform() const;
    virtual glm::mat4 getNodeToParentTransform(Node* ancestor) const;
    virtual glm::mat4 getNodeToWorldTransform() const;
    virtual glm::mat4 getWorldToNodeTransform() const;

    glm::vec2 convertToNodeSpace(const glm::vec2& worldPoint) const;
    glm::vec2 convertToWorldSpace(const glm::vec2& nodePoint) const;

    virtual void visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags);

    virtual Scene* getScene() const;

protected:
    bool isVisitableByVisitingCamera() const;
    glm::mat4 transform(const glm::mat4& parentTransform);
    uint32_t processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag);
    
protected:
    Node* m_pParent;                        //!< 親ノード
    Game* m_pGame;                          //!< シングルトンのゲームクラス
    EventDispatcher* m_pEventDispatcher;    // イベントディスパッチ

    uint16_t m_cameraMask;	//!< カメラマスク

    glm::vec3 m_position;	//!< ノードの位置
    glm::vec2 m_size;		//!< コンテンツサイズ

    glm::vec2 m_anchorPointInPoints;	//!< ポイントのアンカーポイント
    glm::vec2 m_anchorPoint;			//!< 正規化されたアンカーポイント

    float m_rotation;		//!< Z軸の回転角度
    
    float m_scaleX;			//!< X軸の倍率
    float m_scaleY;			//!< Y軸の倍率
    float m_scaleZ;			//!< Z軸の倍率

    mutable glm::mat4 m_transform;			//!< 変換行列
    mutable glm::mat4 m_modelVewTransform;	//!< モデルビュー行列

    std::int32_t m_localZOrder;	//!< ローカルZオーダー
    float m_globalZOrder;		//!< グローバルZオーダー

    bool m_visible;							//!< ノード表示フラグ
    bool m_ignoreAnchorPointForPosition;	//!< ノードの基準点がvec2(0,0)の場合はtrue、それ以外の場合はfalse
    mutable bool m_transformDirty;			//!< 変換のダーティーフラグ
    bool m_transformUpdated;				//!< 変換の更新のダーティーフラグ
    bool m_contentSizeDirty;				//!< コンテンツサイズのダーティーフラグ

    std::vector<Node*> m_children;			//!< 子ノードの配列
    std::vector<Component*> m_components;	//!< コンポーネント
};

bool isScreenPointInRect(const glm::vec2& pt, const Camera* pCamera, const glm::mat4& worldToLocal,
                         const Rect& rect, glm::vec3* p);

NS_OCF_END
