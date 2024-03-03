#include "Sprite.h"
#include "base/Game.h"
#include "renderer/Texture2D.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/ShaderManager.h"

#define SPRITE_DEBUG_DRAW

using namespace glm;

OCF_BEGIN

static unsigned short indices[] = { 0, 1, 2, 3, 2, 1 };

Sprite::Sprite(int drawOrder)
	: m_drawOrder(drawOrder)
	, m_modelView(1.0f)
	, m_flippedX(false)
	, m_flippedY(false)
	, m_texture(nullptr)
{
	m_size = { 2.0f, 2.0f };

	m_quad.topLeft.position		= { 0.0f, 0.0f, 0.0f };
	m_quad.bottomLeft.position	= { 0.0f, 0.0f, 0.0f };
	m_quad.topRight.position	= { 0.0f, 0.0f, 0.0f };
	m_quad.bottomRight.position	= { 0.0f, 0.0f, 0.0f };

	m_quad.topLeft.color		= { 1.0f, 1.0f, 1.0f };
	m_quad.bottomLeft.color		= { 1.0f, 1.0f, 1.0f };
	m_quad.topRight.color		= { 1.0f, 1.0f, 1.0f };
	m_quad.bottomRight.color	= { 1.0f, 1.0f, 1.0f };

	m_quad.topLeft.texCoord		= { 0.0f, 1.0f };
	m_quad.bottomLeft.texCoord	= { 0.0f, 0.0f };
	m_quad.topRight.texCoord	= { 1.0f, 1.0f };
	m_quad.bottomRight.texCoord	= { 1.0f, 0.0f };

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

	ProgramState& programState = m_trianglesCommand.getProgramState();
	programState.init(ShaderManager::getInstance()->getProgram(ProgramType::Basic));

	Program* program = programState.getProgram();
	VertexBuffer* vertexBuffer = programState.getVertexBuffer();
	VertexArray* vertexArray = programState.getVertexArray();

	glUseProgram(program->getProgram());

	glBindVertexArray(vertexArray->getHandle());
	vertexBuffer->bind();

	int location = program->getAttributeLocation("inPosition");
	vertexArray->setAttribute(location, 3, sizeof(Vertex3fC3fT2f), 0);

	location = program->getAttributeLocation("inColor");
	vertexArray->setAttribute(location, 3, sizeof(Vertex3fC3fT2f), sizeof(glm::vec3));
	
	vertexBuffer->unbind();

	glBindVertexArray(0);
	glUseProgram(0);

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
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3fT2f), &m_quad.topLeft.position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3fT2f), &m_quad.topLeft.texCoord);

	// テクスチャを設定
	if (m_texture)
		m_texture->setActive();

	// レンダリング
	static unsigned char indices[] = { 0, 1, 2, 3, 2, 1 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

	// スプライトデバッグ表示
#ifdef SPRITE_DEBUG_DRAW
	glColor3ub(0x00, 0xff, 0x00);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	{
		glVertex2f(0.0f, 0.0f);
	}
	glEnd();

	// ポリンゴンをワイヤーフレームで描画
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glEnd();
#endif

	glPopMatrix();
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

	ProgramState& programState = m_trianglesCommand.getProgramState();
	VertexBuffer* vertexBuffer =  programState.getVertexBuffer();
	vertexBuffer->updateData(&m_quad.topLeft.position, sizeof(m_quad));

	VertexBuffer* indexBuffer = programState.getIndexBuffer();
	indexBuffer->updateData(&indices[0], sizeof(indices));
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
