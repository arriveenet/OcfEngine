#include "ProgramState.h"

OCF_BEGIN

ProgramState::ProgramState()
	: m_pProgram(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pVertexArray(nullptr)
{
}

ProgramState::~ProgramState()
{
	delete m_pVertexBuffer;
	m_pVertexBuffer = nullptr;

	delete m_pVertexBuffer;
	m_pVertexBuffer = nullptr;

	delete m_pVertexArray;
	m_pVertexArray = nullptr;
}

void ProgramState::init(Program* program)
{
	m_pProgram = program;

	m_pVertexBuffer = new VertexBuffer(BufferType::Vertex, BufferUsage::Dynamic);
	m_pIndexBuffer = new VertexBuffer(BufferType::Index, BufferUsage::Dynamic);

	m_pVertexArray = new VertexArray();
}

void ProgramState::createVertexBuffer(void* pData, size_t size)
{
	m_pVertexBuffer->updateData(pData, size);
}

void ProgramState::createIndexBuffer(void* pData, size_t size)
{
	m_pIndexBuffer->updateData(pData, size);
}

OCF_END
