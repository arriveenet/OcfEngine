#include "Sprite.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "base/Game.h"
#include "renderer/Texture2D.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/ShaderManager.h"

#define SPRITE_DEBUG_DRAW	1

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
	, m_isDirty(true)
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

	Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::Basic);
	m_trianglesCommand.getProgramState().setProgram(pProgram);

#if SPRITE_DEBUG_DRAW
	m_pDebugDrawShape = new DrawShape();
	addChild(m_pDebugDrawShape);
#endif
}

Sprite::~Sprite()
{
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

void Sprite::setPosition(const glm::vec2& position)
{
	Entity::setPosition(position);
}

void Sprite::setPosition(float x, float y)
{
	Entity::setPosition(x, y);
}

void Sprite::setSize(float width, float height)
{
	Entity::setSize(width, height);
	updatePolygon();

	m_isDirty = true;
}

Rect Sprite::getRect() const
{
	return m_rect;
}

void Sprite::draw(Renderer* renderer, const glm::mat4& transform)
{
	setMVPMarixUniform();

	m_trianglesCommand.init(m_texture, m_triangles, transform);

	renderer->addCommand(&m_trianglesCommand);

#if SPRITE_DEBUG_DRAW
	m_pDebugDrawShape->clear();

	int count = m_triangles.indexCount / 3;
	auto vertices = m_triangles.vertices;
	auto indices = m_triangles.indices;
	for (int i = 0; i < count; i++) {
		glm::vec3 from = vertices[indices[i * 3]].position;
		glm::vec3 to   = vertices[indices[i * 3 + 1]].position;
		m_pDebugDrawShape->drawLine(glm::vec2(from.x, from.y), glm::vec2(to.x, to.y), Color4f::WHITE);

		from = vertices[indices[i * 3 + 1]].position;
		to   = vertices[indices[i * 3 + 2]].position;
		m_pDebugDrawShape->drawLine(glm::vec2(from.x, from.y), glm::vec2(to.x, to.y), Color4f::WHITE);

		from = vertices[indices[i * 3 + 2]].position;
		to   = vertices[indices[i * 3]].position;
		m_pDebugDrawShape->drawLine(glm::vec2(from.x, from.y), glm::vec2(to.x, to.y), Color4f::WHITE);
	}
#endif
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
	m_quad.bottomLeft.position = { lb , 0.0f };
	m_quad.topRight.position = { rt, 0.0f };
	m_quad.bottomRight.position = { rt.x, lb.y, 0.0f };

	m_triangles.vertices = &m_quad.topLeft;
	m_triangles.vertexCount = 4;
	m_triangles.indices = indices;
	m_triangles.indexCount = 6;
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

void Sprite::setMVPMarixUniform()
{
	glm::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
	auto& programState = m_trianglesCommand.getProgramState();
	programState.setUniform("uViewProj", projection);
	programState.setUniform("uWorldTransform", glm::mat4(1.0f));
}

OCF_END
