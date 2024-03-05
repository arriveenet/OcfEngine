#pragma once
#include <string>
#include "Component.h"
#include "Entity.h"
#include "base/types.h"
#include "base/Rect.h"
#include "renderer/TrianglesCommand.h"
#include "renderer/VertexArray.h"

OCF_BEGIN

class Texture2D;

class Sprite : public Entity {
public:
	static Sprite* create(const std::string& filenam, int drawOrder = 100);

	Sprite(int drawOrder = 100);
	virtual ~Sprite();

	virtual bool init();
	virtual bool initWithFile(const std::string& filename);

	int getDrawOrder() const;
	
	void setPosition(float x, float y) override;

	void setSize(float width, float height) override;

	virtual Rect getRect() const;

	virtual void draw();
	void draw(Renderer* renderer) override;

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
	QuadV3fC3fT2f m_quad;
	Rect m_rect;
	glm::mat4 m_modelView;

	bool m_flippedX;
	bool m_flippedY;

	Texture2D* m_texture;
	TrianglesCommand m_trianglesCommand;
	VertexArray m_vertexArray;
};

OCF_END
