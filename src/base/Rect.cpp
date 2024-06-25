#include "Rect.h"

NS_OCF_BEGIN

Rect::Rect()
{
	setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

Rect::Rect(float x, float y, float widht, float height)
{
	setRect(x, y, widht, height);
}

Rect::Rect(const glm::vec2& position, const glm::vec2& size)
{
	setRect(position.x, position.y, size.x, size.y);
}

Rect& Rect::operator=(const Rect& other)
{
	setRect(other.m_position.x, other.m_position.y, other.m_size.x, other.m_size.y);
	return *this;
}

void Rect::setRect(float x, float y, float widht, float height)
{
	m_position.x = x;
	m_position.y = y;

	m_size.x = widht;
	m_size.y = height;
}

float Rect::getMaxX() const
{
	return m_position.x + m_size.x;
}

float Rect::getMaxY() const
{
	return m_position.y + m_size.y;
}

float Rect::getMinX() const
{
	return m_position.x;
}

float Rect::getMinY() const
{
	return m_position.y;
}

bool Rect::equals(const Rect& rect) const
{
	return (m_position == rect.m_position && m_size == rect.m_size);
}

bool Rect::intersect(const glm::vec2& point) const
{
	if (point.x >= getMinX() && point.x <= getMaxX() && point.y >= getMinY() && point.y <= getMaxY()) {
		return true;
	}

	return false;
}

bool Rect::intersect(const Rect& rect) const
{
	if ((getMaxX() > rect.getMinX())
		&& (getMinX() < rect.getMaxX())
		&& (getMaxY() > rect.getMinY())
		&& (getMinY() < rect.getMaxY())
		) {
		return true;
	}
	return false;
}

NS_OCF_END
