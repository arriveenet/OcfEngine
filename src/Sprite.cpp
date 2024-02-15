#include "Sprite.h"
#include "Game.h"
#include <GLFW/glfw3.h>

Sprite::Sprite(int drawOrder)
	: m_drawOrder(drawOrder)
	, m_flippedX(false)
	, m_flippedY(false)
{
	m_size = { 100, 100 };
	Game::getInstance()->getRenderer()->addSprite(this);
}

Sprite::~Sprite()
{
}

bool Sprite::init()
{
	return true;
}

void Sprite::setPosition(float x, float y)
{
	Entity::setPosition(x, y);
}

Rect Sprite::getRect() const
{
	return m_rect;
}

void Sprite::draw()
{
	updatePolygon();
	
	glColor3ub(0xff, 0xff, 0xff);
	glVertexPointer(2, GL_FLOAT, 0, m_vertices);
	glDrawArrays(GL_QUADS, 0, 4);

	glColor3ub(0x00, 0xff, 0x00);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	{
		glVertex2f(m_position.x, m_position.y);
	}
	glEnd();

	glBegin(GL_LINES);
	{
		glVertex2fv((GLfloat*)&m_vertices[0]);
		glVertex2fv((GLfloat*)&m_vertices[1]);
		glVertex2fv((GLfloat*)&m_vertices[1]);
		glVertex2fv((GLfloat*)&m_vertices[2]);
		glVertex2fv((GLfloat*)&m_vertices[2]);
		glVertex2fv((GLfloat*)&m_vertices[3]);
		glVertex2fv((GLfloat*)&m_vertices[3]);
		glVertex2fv((GLfloat*)&m_vertices[0]);
	}
	glEnd();
	//glBegin(GL_QUADS);
	//{
	//	glTexCoord2f(0.0f, 1.0f);
	//	glVertex2f(m_rect.getMinX(), m_rect.getMinY());
	//	glTexCoord2f(0.0f, 0.0f);
	//	glVertex2f(m_rect.getMinX(), m_rect.getMaxY());
	//	glTexCoord2f(1.0f, 0.0f);
	//	glVertex2f(m_rect.getMaxX(), m_rect.getMaxY());
	//	glTexCoord2f(1.0f, 1.0f);
	//	glVertex2f(m_rect.getMaxX(), m_rect.getMinY());
	//}
	//glEnd();
}

void Sprite::setFlippedX(bool flippedX)
{
	if (m_flippedX != flippedX) {
		m_flippedX = flippedX;
		flipX();
	}
}

void Sprite::setFlippedY(bool flippedY)
{
	if (m_flippedY != flippedY) {
		m_flippedY = flippedY;
		flipY();
	}
}

bool Sprite::isFlippedX() const
{
	return m_flippedX;
}

bool Sprite::isFlippedY() const
{
	return m_flippedY;
}

void Sprite::updatePolygon()
{
	float w = m_size.x / 2.0f;
	float h = m_size.y / 2.0f;
	glm::vec2 a = { m_position.x - w, m_position.y - h };
	glm::vec2 b = { m_position.x + w, m_position.y + h };

	m_vertices[0] = a;
	m_vertices[1] = { a.x, b.y };
	m_vertices[2] = b;
	m_vertices[3] = { b.x, a.y };
}

void Sprite::flipX()
{
}

void Sprite::flipY()
{
}
