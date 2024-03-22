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
	, m_bufferCapacityLine(0)
	, m_bufferCountLine(0)
{
	init();
}

DrawShape::~DrawShape()
{
}

bool DrawShape::init()
{
	m_customCommandLine.setDrawType(CustomCommand::DrawType::Array);
	m_customCommandLine.setPrimitiveType(PrimitiveType::Line);

	Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::DrawShape);

	auto& programState = m_customCommandLine.getProgramState();
	programState.setProgram(pProgram);

	VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
	pVertexArray->bind();

	pVertexArray->setStride(sizeof(Vertex2fC4));

	pVertexArray->createVertexBuffer(BufferUsage::Dynamic);
	ensureCapacityGLLine(256);

	pVertexArray->setAttribute("inPosition", 0, 2, false, 0);
	pVertexArray->setAttribute("inColor", 1, 4, false, sizeof(float) * 2);

	pVertexArray->bindVertexBuffer();

	pVertexArray->unbind();

	return true;
}

void DrawShape::clear()
{
	m_lineBuffers.clear();
	m_bufferCountLine = 0;
	m_dirtyLine = true;
}

void DrawShape::ensureCapacityGLLine(int count)
{
	if (m_bufferCountLine + count > m_bufferCapacityLine) {
		m_bufferCapacityLine += max(m_bufferCapacityLine, count);
		m_lineBuffers.resize(m_bufferCapacityLine);

		VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
		pVertexArray->updateVertexBuffer(m_lineBuffers.data(), sizeof(Vertex2fC4) * m_bufferCapacityLine);
	}
}

void DrawShape::drawLine(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color)
{
	ensureCapacityGLLine(2);

	m_lineBuffers.push_back({ origin, color });
	m_lineBuffers.push_back({ destanation, color });

	VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
	pVertexArray->updateVertexBuffer(m_lineBuffers.data() + m_bufferCountLine, sizeof(Vertex2fC4) * m_bufferCountLine, sizeof(Vertex2fC4) * 2);

	m_bufferCountLine += 2;
	m_dirtyLine = true;

	m_customCommandLine.setVertexDrawInfo(0, m_bufferCountLine);
}

void DrawShape::update(float deltaTime)
{
}

void DrawShape::draw(Renderer* renderer, const glm::mat4& transform)
{
	if (!m_lineBuffers.empty()) {
		m_customCommandLine.init(transform);
		renderer->addCommand(&m_customCommandLine);
	}

}

void DrawShape::updateVertexBuffer()
{
}

OCF_END
