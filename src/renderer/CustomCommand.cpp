#include "CustomCommand.h"

OCF_BEGIN
CustomCommand::CustomCommand()
	: m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_drawType(DrawType::Element)
	, m_primitiveType(PrimitiveType::Triangle)
{
	m_Type = Type::CustomCommand;
	m_pVertexArray = new VertexArray();
}

CustomCommand::~CustomCommand()
{
	OCF_SAFE_DELETE(m_pVertexArray);
	OCF_SAFE_DELETE(m_pVertexBuffer);
	OCF_SAFE_DELETE(m_pIndexBuffer);
}

void CustomCommand::init(const glm::mat4& modelView)
{
	RenderCommand::init(modelView);
}

void CustomCommand::createVertexBuffer(BufferUsage usage)
{
	OCF_SAFE_DELETE(m_pVertexBuffer);

	m_pVertexBuffer = new VertexBuffer(BufferType::Vertex, usage);
}

void CustomCommand::createIndexBuffer(BufferUsage usage)
{
	OCF_SAFE_DELETE(m_pIndexBuffer);

	m_pIndexBuffer = new VertexBuffer(BufferType::Index, usage);
}

void CustomCommand::updateVertexBuffer(void* pData, size_t size)
{
	assert(m_pVertexBuffer != nullptr);

	m_pVertexBuffer->updateData(pData, size);
}

void CustomCommand::updateIndexBuffer(void* pData, size_t size)
{
	assert(m_pIndexBuffer != nullptr);

	m_pIndexBuffer->updateData(pData, size);
}

OCF_END
