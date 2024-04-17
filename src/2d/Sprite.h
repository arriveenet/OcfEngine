#pragma once
#include <string>
#include "Component.h"
#include "Entity.h"
#include "DrawShape.h"
#include "base/types.h"
#include "base/Rect.h"
#include "renderer/TrianglesCommand.h"
#include "renderer/VertexArray.h"

OCF_BEGIN

class Texture2D;

/**
 * @brief �X�v���C�g�N���X
 */
class Sprite : public Entity {
public:
	/** �X�v���C�g���쐬*/
	static Sprite* create(const std::string& filenam, int drawOrder = 100);

	/** �R���X�g���N�^ */
	Sprite(int drawOrder = 100);
	/** �f�X�g���N�^ */
	virtual ~Sprite();

	/** �X�v���C�g�������� */
	virtual bool init();
	/** �X�v���C�g���t�@�C�����珉���� */
	virtual bool initWithFile(const std::string& filename);

	/** �`�揇�����擾 */
	int getDrawOrder() const;
	
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
	void flipX();
	void flipY();

protected:
	int m_drawOrder;
	bool m_isDirty;
	QuadV3fC3fT2f m_quad;
	Rect m_rect;
	glm::mat4 m_modelView;

	bool m_flippedX;
	bool m_flippedY;

	Texture2D* m_texture;
	TrianglesCommand::Triangles m_triangles;
	TrianglesCommand m_trianglesCommand;
	DrawShape m_drawShape;
};

OCF_END
