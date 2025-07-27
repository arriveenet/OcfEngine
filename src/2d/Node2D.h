#pragma once
#include "base/CanvasItem.h"

NS_OCF_BEGIN

class Renderer;

class Node2D : public CanvasItem {
public:
    Node2D();
    ~Node2D();

    virtual void setPosition(const glm::vec2& position);
    virtual void setSize(const glm::vec2& size);
    virtual void setRotation(float rotation);
    virtual void setScale(const glm::vec2& scale);

    virtual glm::vec2 getPosition() const;
    virtual float getRotation() const;
    virtual glm::vec2 getSize() const;
    virtual glm::vec2 getScale() const;

    void setAnchorPoint(const glm::vec2& point);
    const glm::vec2& getAnchorPoint() const;

    bool hitTest(const glm::vec2& worldPoint) const;

    const glm::mat4& getNodeToParentTransform() const;
    glm::mat4 getNodeToParentTransform(Node* ancestor) const;
    glm::mat4 getNodeToWorldTransform() const;
    glm::mat4 getWorldToNodeTransform() const;

    glm::vec2 convertToNodeSpace(const glm::vec2& worldPoint) const;
    glm::vec2 convertToWorldSpace(const glm::vec2& nodePoint) const;

    void visit(Renderer* pRenderer, const glm::mat4& parentTransform,
               uint32_t parentFlags) override;

protected:
    glm::mat4 transform(const glm::mat4& parentTransform) const;
    uint32_t processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag);

protected:
    glm::vec2 m_position;                   //!< ノードの位置
    glm::vec2 m_size;                       //!< コンテンツサイズ
    float m_rotation;                       //!< 回転角度
    glm::vec2 m_scale;                      //!< スケール倍率

    glm::vec2 m_anchorPointInPoints;        //!< ポイントのアンカーポイント
    glm::vec2 m_anchorPoint;                //!< 正規化されたアンカーポイント

    mutable glm::mat4 m_transform;          //!< 変換行列
    mutable glm::mat4 m_modelVewTransform;  //!< モデルビュー行列

    bool m_ignoreAnchorPointForPosition;    //!< ノードの基準点がvec2(0,0)の場合はtrue、それ以外の場合はfalse
    mutable bool m_transformDirty;          //!< 変換のダーティーフラグ
    bool m_transformUpdated;                //!< 変換の更新のダーティーフラグ
    bool m_contentSizeDirty;                //!< コンテンツサイズのダーティーフラグ

};

NS_OCF_END
