#include "Sprite.h"
#include "Game.h"
#include <GLFW/glfw3.h>

Sprite::Sprite(Entity* pEntity, int drawOrder)
	: Component(pEntity)
	, m_drawOrder(drawOrder)
{
	Game::getInstance()->getRenderer()->addSprite(this);

	m_rect.setRect(10, 10, 100, 100);
}

Sprite::~Sprite()
{
}

bool Sprite::init()
{
	m_rect.setRect(10, 10, 100, 100);
	return true;
}

void Sprite::setPosition(float x, float y)
{
	m_rect.m_position.x = x;
	m_rect.m_position.y = y;
}

Rect Sprite::getRect() const
{
	return m_rect;
}

void Sprite::draw()
{
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(m_rect.getMinX(), m_rect.getMinY());
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(m_rect.getMinX(), m_rect.getMaxY());
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(m_rect.getMaxX(), m_rect.getMaxY());
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(m_rect.getMaxX(), m_rect.getMinY());
	}
	glEnd();
}
