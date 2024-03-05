#include "Sprite.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "base/Game.h"
#include "renderer/Texture2D.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/ShaderManager.h"

#define SPRITE_DEBUG_DRAW

using namespace glm;

OCF_BEGIN

static unsigned short indices[] = { 0, 1, 2, 3, 2, 1 };

Sprite* Sprite::create(const std::string& filenam, int drawOrder/*= 100*/)
{
	Sprite* pSprite = new Sprite(drawOrder);

	if (pSprite) {
		pSprite->initWithFile(filenam);
		return pSprite;
	}

	return nullptr;
}

Sprite::Sprite(int drawOrder)
	: m_drawOrder(drawOrder)
	, m_modelView(1.0f)
	, m_flippedX(false)
	, m_flippedY(false)
	, m_texture(nullptr)
{
	m_size = { 2.0f, 2.0f };

	// 頂点を設定
	m_quad.topLeft.position		= { -0.5f,  0.5f, 0.0f };
	m_quad.bottomLeft.position	= { -0.5f, -0.5f, 0.0f };
	m_quad.topRight.position	= {  0.5f,  0.5f, 0.0f };
	m_quad.bottomRight.position	= {  0.5f, -0.5f, 0.0f };

	// 色を設定
	m_quad.topLeft.color		= { 1.0f, 1.0f, 1.0f };
	m_quad.bottomLeft.color		= { 1.0f, 1.0f, 1.0f };
	m_quad.topRight.color		= { 1.0f, 1.0f, 1.0f };
	m_quad.bottomRight.color	= { 1.0f, 1.0f, 1.0f };

	// テクスチャ座標を設定
	m_quad.topLeft.texCoord		= { 0.0f, 0.0f };
	m_quad.bottomLeft.texCoord	= { 0.0f, 1.0f };
	m_quad.topRight.texCoord	= { 1.0f, 0.0f };
	m_quad.bottomRight.texCoord	= { 1.0f, 1.0f };

	m_vertexArray.init(&m_quad.topLeft, indices, 4, 6);

	Game::getInstance()->getRenderer()->addSprite(this);
}

Sprite::~Sprite()
{
	Game::getInstance()->getRenderer()->removeSprite(this);
}

bool Sprite::init()
{
	return true;
}

bool Sprite::initWithFile(const std::string& filename)
{
	m_texture = Game::getInstance()->getTextureManager()->addImage(filename);

	if (!m_texture) {
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
	Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::Basic);
	glUseProgram(pProgram->getProgram());

	Scene* scene = Game::getInstance()->getCurrentScene();
	glm::mat4 projection = scene->getDefaultCamera()->getProjectionMatrix();

	GLint location = glGetUniformLocation(pProgram->getProgram(), "uViewProj");
	glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(projection));

	glm::mat4 modelView(1.0f);
	modelView *= glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0));
	modelView *= glm::scale(glm::mat4(1.0f), glm::vec3(m_size.x, m_size.y, 1.0f));
	location = glGetUniformLocation(pProgram->getProgram(), "uWorldTransform");
	glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(modelView));

	if (m_texture)
		m_texture->setActive();

	m_vertexArray.bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Sprite::draw(Renderer* renderer)
{
	TrianglesCommand::Triangles triangles;
	triangles.vertices = &m_quad.bottomLeft;
	triangles.vertexCount = 4;
	triangles.indices = indices;
	triangles.indexCount = 6;

	m_trianglesCommand.init(m_texture, triangles, m_modelView);

	renderer->addCommand(&m_trianglesCommand);
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

	m_quad.topLeft.position = { lb.x, rt.y, 0.0f };
	m_quad.bottomLeft.position = { lb , 0.0f};
	m_quad.topRight.position = { rt, 0.0f };
	m_quad.bottomRight.position = { rt.x, lb.y, 0.0f };
}

void Sprite::flipX()
{
	std::swap(m_quad.topLeft.texCoord, m_quad.topRight.texCoord);
	std::swap(m_quad.bottomLeft.texCoord, m_quad.bottomRight.texCoord);
}

void Sprite::flipY()
{
	std::swap(m_quad.topLeft.texCoord, m_quad.bottomLeft.texCoord);
	std::swap(m_quad.topRight.texCoord, m_quad.bottomRight.texCoord);
}

OCF_END
