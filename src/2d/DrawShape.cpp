#include "DrawShape.h"
#include "base/Game.h"
#include "renderer/ShaderManager.h"

OCF_BEGIN

DrawShape* DrawShape::create()
{
	static DrawShape drawShape;
	return &drawShape;
}

DrawShape::DrawShape()
	: m_dirtyLine(false)
	, m_bufferCapacity(0)
{
	init();
}

DrawShape::~DrawShape()
{
}

bool DrawShape::init()
{
	m_vertexArray.bind();

	m_vertexArray.setStride(sizeof(Vertex2fC4));

	m_vertexArray.updateVertexData(m_lineBuffers.data(), sizeof(Vertex3fC3fT2f) * m_lineBuffers.size());

	m_vertexArray.setAttribute("inPosition", 0, 3, false, 0);
	m_vertexArray.setAttribute("inColor", 1, 4, false, sizeof(float) * 4);

	m_vertexArray.unbind();

	return true;
}

void DrawShape::drawLine(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color)
{
	m_lineBuffers.push_back({ origin, color });
	m_lineBuffers.push_back({ destanation, color });

	m_dirtyLine = true;
}

void DrawShape::update(float deltaTime)
{
}

void DrawShape::draw(Renderer* renderer)
{
	if (!m_lineBuffers.empty()) {
		m_customCommand.init(m_transform);
		renderer->addCommand(&m_customCommand);
	}

}

void DrawShape::updateVertexBuffer()
{
}

OCF_END
