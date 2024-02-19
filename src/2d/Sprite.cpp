#include "Sprite.h"
#include "Game.h"
#include "renderer/Texture2D.h"
#include <GLFW/glfw3.h>

#define SPRITE_DEBUG

using namespace glm;

Sprite::Sprite(int drawOrder)
	: m_drawOrder(drawOrder)
	, m_flippedX(false)
	, m_flippedY(false)
	, m_texture(nullptr)
{
	m_size = { 2.0f, 2.0f };
	m_vertices[0].position = { -1.0f, -1.0f };
	m_vertices[1].position = {  1.0f, -1.0f };
	m_vertices[2].position = {  1.0f,  1.0f };
	m_vertices[3].position = { -1.0f,  1.0f };

	m_vertices[0].texCoord = { 0.0f, 0.0f };	// 左下
	m_vertices[1].texCoord = { 1.0f, 0.0f };	// 右下
	m_vertices[2].texCoord = { 1.0f, 1.0f };	// 右上
	m_vertices[3].texCoord = { 0.0f, 1.0f };	// 左上

	Game::getInstance()->getRenderer()->addSprite(this);
}

Sprite::~Sprite()
{
	delete m_texture;
	m_texture = nullptr;
}

bool Sprite::init()
{
	return true;
}

bool Sprite::initWithFile(const std::string& filename)
{
	m_texture = new Texture2D();
	if (!m_texture->initWithFile(filename)) {
		delete m_texture;
		m_texture = nullptr;
		return false;
	}

	setSize((float)m_texture->getWidth(), (float)m_texture->getHeight());

	return true;
}

int Sprite::getDrawOrder() const
{
	return m_drawOrder;
}

void Sprite::setPosition(float x, float y)
{
	Entity::setPosition(x, y);
}

void Sprite::setSize(float width, float height)
{
	Entity::setSize(width, height);
	updatePolygon();
}

Rect Sprite::getRect() const
{
	return m_rect;
}

void Sprite::draw()
{
	glPushMatrix();

	// 平行移動
	glTranslatef(m_position.x, m_position.y, 0.0f);
	// 回転
	glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
	// 拡大縮小
	glScalef(m_scaleX, m_scaleY, m_scaleZ);

	glColor3ub(0xff, 0xff, 0xff);

	// 頂点データの配列を定義
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex2f), &m_vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2f), &m_vertices[0].texCoord);

	// テクスチャを設定
	if (m_texture)
		m_texture->setActive();

	// レンダリング
	glDrawArrays(GL_QUADS, 0, 4);

	// スプライトデバッグ表示
#ifdef SPRITE_DEBUG
	glColor3ub(0x00, 0xff, 0x00);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	{
		glVertex2f(0.0f, 0.0f);
	}
	glEnd();

	glBegin(GL_LINES);
	{
		glVertex2fv((GLfloat*)&m_vertices[0].position);
		glVertex2fv((GLfloat*)&m_vertices[1].position);
		glVertex2fv((GLfloat*)&m_vertices[1].position);
		glVertex2fv((GLfloat*)&m_vertices[2].position);
		glVertex2fv((GLfloat*)&m_vertices[2].position);
		glVertex2fv((GLfloat*)&m_vertices[3].position);
		glVertex2fv((GLfloat*)&m_vertices[3].position);
		glVertex2fv((GLfloat*)&m_vertices[0].position);
	}
	glEnd();
#endif

	glPopMatrix();
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
	float offsetX = m_size.x / 2.0f;
	float offsetY = m_size.y / 2.0f;
	glm::vec2 lb = { m_position.x - offsetX, m_position.y - offsetY };
	glm::vec2 rt = { m_position.x + offsetX, m_position.y + offsetY };
	
	m_vertices[0].position = lb;
	m_vertices[1].position = { rt.x, lb.y };
	m_vertices[2].position = rt;
	m_vertices[3].position = { lb.x, rt.y };
}

void Sprite::flipX()
{
	std::swap(m_vertices[0].texCoord, m_vertices[1].texCoord);
	std::swap(m_vertices[2].texCoord, m_vertices[3].texCoord);
}

void Sprite::flipY()
{
	std::swap(m_vertices[0].texCoord, m_vertices[3].texCoord);
	std::swap(m_vertices[1].texCoord, m_vertices[2].texCoord);
}
