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
 * @brief スプライトクラス
 */
class Sprite : public Entity {
public:
	/** スプライトを作成*/
	static Sprite* create(const std::string& filenam, int drawOrder = 100);

	/** コンストラクタ */
	Sprite(int drawOrder = 100);
	/** デストラクタ */
	virtual ~Sprite();

	/** スプライトを初期化 */
	virtual bool init();
	/** スプライトをファイルから初期化 */
	virtual bool initWithFile(const std::string& filename);

	/** 描画順序を取得 */
	int getDrawOrder() const;
	
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
