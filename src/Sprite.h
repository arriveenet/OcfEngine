#pragma once
#include "Component.h"
#include "Rect.h"
#include "Entity.h"
#include "base/types.h"
#include <string>

class Texture2D;

class Sprite : public Entity {
public:
	Sprite(int drawOrder = 100);
	virtual ~Sprite();

	virtual bool init();
	virtual bool initWithFile(const std::string& filename);
	
	void setPosition(float x, float y) override;

	void setSize(float width, float height) override;

	virtual Rect getRect() const;

	virtual void draw();

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
	Rect m_rect;
	bool m_flippedX;
	bool m_flippedY;
	Vertex2f m_vertices[4];
	Texture2D* m_texture;
};