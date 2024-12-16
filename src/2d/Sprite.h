#pragma once
#include <string>
#include "Component.h"
#include "Node.h"
#include "DrawShape.h"
#include "base/Types.h"
#include "base/Rect.h"
#include "base/Config.h"
#include "renderer/TrianglesCommand.h"
#include "renderer/VertexArray.h"

NS_OCF_BEGIN

class Texture2D;
class SpriteFrame;

/**
 * @brief スプライトクラス
 */
class Sprite : public Node {
public:
    /** スプライトを作成*/
    static Sprite* create();
    static Sprite* create(const std::string& filenam);
    static Sprite* createWithSpriteFrame(SpriteFrame* spriteFrame);
    static Sprite* createWithSpriteFrameName(std::string_view spriteFrameName);

    /** コンストラクタ */
    Sprite();
    /** デストラクタ */
    virtual ~Sprite();

    /** スプライトを初期化 */
    virtual bool init();
    /** ファイルからスプライトを初期化 */
    virtual bool initWithFile(const std::string& filename);
    /** テクスチャからスプライトを初期化 */
    virtual bool initWithTexture(Texture2D* texture, const Rect& rect);
    /** スプライトフレームからスプライトを初期化 */
    virtual bool initWithSpriteFrame(SpriteFrame* spriteFrame);

    /** テクスチャを設定 */
    virtual void setTexture(Texture2D* texture);
    
    /** スプライトフレームを設定 */
    virtual void setSpriteFrame(SpriteFrame* spriteFrame);
    /** スプライトフレームを取得 */
    virtual SpriteFrame* getSpriteFrame() const;
    
    /** スプライトの位置を設定 */
    void setPosition(const glm::vec2& position) override;
    void setPosition(float x, float y) override;

    /** スプライトのサイズを設定 */
    void setSize(float width, float height) override;

    /** スプライトの矩形を取得 */
    virtual Rect getRect() const;

    /** スプライトを描画 */
    void draw(Renderer* renderer, const glm::mat4& transform) override;

    /** スプライトを反転 */
    void setFlippedX(bool flippedX);
    void setFlippedY(bool flippedY);
    bool isFlippedX() const;
    bool isFlippedY() const;

protected:
    void updatePolygon();
    void setTextureRect(const Rect& rect, const glm::vec2& size);
    void setVertexRect(const Rect& rect);
    void setTextureCoords(const Rect& rectInPoints, QuadV3fC3fT2f* outQuad);
    void setVertexCoords(const Rect& rect, QuadV3fC3fT2f* outQuad);
    void flipX();
    void flipY();
    void setMVPMarixUniform();

protected:
    bool m_isDirty;
    QuadV3fC3fT2f m_quad;
    Rect m_rect;
    glm::mat4 m_modelView;

    bool m_flippedX;
    bool m_flippedY;

    Texture2D* m_texture;
    SpriteFrame* m_spriteFrame;
    TrianglesCommand::Triangles m_triangles;
    TrianglesCommand m_trianglesCommand;

#if OCF_SPRITE_DEBUG_DRAW
    DrawShape* m_pDebugDrawShape = nullptr;
#endif
};

NS_OCF_END
