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
 * @brief �X�v���C�g�N���X
 */
class Sprite : public Node {
public:
    /** �X�v���C�g���쐬*/
    static Sprite* create();
    static Sprite* create(const std::string& filenam);
    static Sprite* createWithSpriteFrame(SpriteFrame* spriteFrame);
    static Sprite* createWithSpriteFrameName(std::string_view spriteFrameName);

    /** �R���X�g���N�^ */
    Sprite();
    /** �f�X�g���N�^ */
    virtual ~Sprite();

    /** �X�v���C�g�������� */
    virtual bool init();
    /** �t�@�C������X�v���C�g�������� */
    virtual bool initWithFile(const std::string& filename);
    /** �e�N�X�`������X�v���C�g�������� */
    virtual bool initWithTexture(Texture2D* texture, const Rect& rect);
    /** �X�v���C�g�t���[������X�v���C�g�������� */
    virtual bool initWithSpriteFrame(SpriteFrame* spriteFrame);

    /** �e�N�X�`����ݒ� */
    virtual void setTexture(Texture2D* texture);
    
    /** �X�v���C�g�t���[����ݒ� */
    virtual void setSpriteFrame(SpriteFrame* spriteFrame);
    /** �X�v���C�g�t���[�����擾 */
    virtual SpriteFrame* getSpriteFrame() const;
    
    /** �X�v���C�g�̈ʒu��ݒ� */
    void setPosition(const glm::vec2& position) override;
    void setPosition(float x, float y) override;

    /** �X�v���C�g�̃T�C�Y��ݒ� */
    void setSize(float width, float height) override;

    /** �X�v���C�g�̋�`���擾 */
    virtual Rect getRect() const;

    /** �X�v���C�g��`�� */
    void draw(Renderer* renderer, const glm::mat4& transform) override;

    /** �X�v���C�g�𔽓] */
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
