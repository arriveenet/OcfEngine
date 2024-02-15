#pragma once
#include "Component.h"
#include "Rect.h"
#include "Entity.h"

class Sprite : public Entity {
public:
	Sprite(int drawOrder = 100);
	virtual ~Sprite();

	virtual bool init();
	
	void setPosition(float x, float y) override;

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
	glm::vec2 m_vertices[4];
};