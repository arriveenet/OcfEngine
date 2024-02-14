#pragma once
#include "Component.h"
#include "Rect.h"
#include "Entity.h"

class Sprite : public Component {
public:
	Sprite(Entity* pEntity, int drawOrder = 100);
	virtual ~Sprite();

	virtual bool init();
	
	virtual void setPosition(float x, float y);

	virtual Rect getRect() const;

	virtual void draw();

protected:
	int m_drawOrder;
	Rect m_rect;

};