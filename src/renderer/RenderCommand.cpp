#include "RenderCommand.h"

OCF_BEGIN

void RenderCommand::init(const glm::mat4& modelViewMatrix)
{
	m_modelVew = modelViewMatrix;
}

RenderCommand::RenderCommand()
{
}

RenderCommand::~RenderCommand()
{
}

TrianglesCommand::TrianglesCommand()
	: m_texture(nullptr)
	, m_program(nullptr)
	, m_vertexBuffer(nullptr)
	, m_vertexArray(nullptr)
{
}

TrianglesCommand::~TrianglesCommand()
{
	delete m_vertexBuffer;
	m_vertexBuffer = nullptr;

	delete m_vertexArray;
	m_vertexArray = nullptr;
}

void TrianglesCommand::init(Texture2D* texture, Program* program, const Triangles& triangles, const glm::mat4& modelView)
{
	RenderCommand::init(modelView);

	m_texture = texture;
	m_program = program;
	m_triangles = triangles;

	m_vertexBuffer = new VertexBuffer(BufferType::Index, BufferUsage::Dynamic);
	m_vertexBuffer->updateData(m_triangles.vertices, m_triangles.vertexCount * sizeof(QuadV3fT2f));

	m_vertexArray = new VertexArray();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TrianglesCommand::draw()
{
	glUseProgram(m_program->getProgram());

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer->getBuffer());
	
	glDrawArrays(GL_TRIANGLES, 0, 0);
}

OCF_END
