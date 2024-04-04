#include "Sprite.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "base/Game.h"
#include "renderer/Texture2D.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/ShaderManager.h"

#define SPRITE_DEBUG_DRAW	0

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

	m_vertexArray.bind();

	m_vertexArray.setStride(sizeof(Vertex3fC3fT2f));

	m_vertexArray.createVertexBuffer(BufferUsage::Dynamic);
	m_vertexArray.createIndexBuffer(BufferUsage::Dynamic);

	m_vertexArray.updateVertexBuffer(&m_quad.topLeft, sizeof(Vertex3fC3fT2f) * 4);
	m_vertexArray.updateIndexBuffer(indices, sizeof(unsigned short)* 6);

	m_vertexArray.setAttribute("inPosition", 0, 3, false, 0);
	m_vertexArray.setAttribute("inNormal", 1, 3, false, sizeof(float) * 3);
	m_vertexArray.setAttribute("inTexCoord", 2, 2, false, sizeof(float) * 6);

	m_vertexArray.bindVertexBuffer();

	m_vertexArray.unbind();

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

	m_isDirty = true;
}

Rect Sprite::getRect() const
{
	return m_rect;
}

void Sprite::updateEntity(float deltaTime)
{
	if (m_isDirty) {
		m_vertexArray.updateVertexBuffer(&m_quad.topLeft, sizeof(Vertex3fC3fT2f) * 4);
		m_vertexArray.updateIndexBuffer(indices, sizeof(unsigned short) * 6);

		m_isDirty = false;
	}

}

void Sprite::draw()
{
	// シェーダを設定
	Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::Basic);
	glUseProgram(pProgram->getProgram());

	// プロジェクション行列、モデルビュー行列を設定
	Scene* scene = Game::getInstance()->getCurrentScene();
	glm::mat4 projection = scene->getDefaultCamera()->getProjectionMatrix();
	glm::mat4 view = scene->getDefaultCamera()->getViewMatrix();

	updateTransform();

	glm::mat4 modelView = view * m_transform;

	// 行列をシェーダに設定
	pProgram->setUniform("uViewProj", projection);
	pProgram->setUniform("uWorldTransform", modelView);

	// テクスチャを設定
	if (m_texture)
		m_texture->setActive();

	// VAOを設定
	m_vertexArray.bind();

	// 描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

	m_vertexArray.unbind();
}

void Sprite::draw(Renderer* renderer, const glm::mat4& transform)
{
	TrianglesCommand::Triangles triangles;
	triangles.vertices = &m_quad.topLeft;
	triangles.vertexCount = 4;
	triangles.indices = indices;
	triangles.indexCount = 6;

	updateTransform();

	m_trianglesCommand.init(m_texture, triangles, m_modelView);

	renderer->addCommand(&m_trianglesCommand);

#if SPRITE_DEBUG_DRAW
	m_drawShape.clear();
	m_drawShape.drawLine(glm::vec2(m_quad.topLeft.position), glm::vec2(m_quad.bottomLeft.position), Color4f::WHITE);
	m_drawShape.drawLine(glm::vec2(m_quad.bottomLeft.position), glm::vec2(m_quad.bottomRight.position), Color4f::WHITE);
	m_drawShape.drawLine(glm::vec2(m_quad.bottomRight.position), glm::vec2(m_quad.topRight.position), Color4f::WHITE);
	m_drawShape.drawLine(glm::vec2(m_quad.topRight.position), glm::vec2(m_quad.topLeft.position), Color4f::WHITE);

	m_drawShape.draw(renderer, m_transform);
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
