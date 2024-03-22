#include "VertexBuffer.h"

OCF_BEGIN

namespace {
	GLenum toOpenGLUsage(BufferUsage usage)
	{
		switch (usage) {
		case BufferUsage::Static:
			return GL_STATIC_DRAW;
			break;
		case BufferUsage::Dynamic:
			return GL_DYNAMIC_DRAW;
			break;
		default:
			return GL_DYNAMIC_DRAW;
			break;
		}
	}
}

VertexBuffer::VertexBuffer(BufferType type, BufferUsage usage)
	: m_buffer(0)
	, m_type(type)
	, m_usage(usage)
{
	glGenBuffers(1, &m_buffer);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

void VertexBuffer::bind() const
{
	if (m_type == BufferType::Vertex) {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	}
}

void VertexBuffer::unbind() const
{
	if (m_type == BufferType::Vertex) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void VertexBuffer::updateData(void* pData, size_t size)
{
	if (m_type == BufferType::Vertex) {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ARRAY_BUFFER, size, pData, toOpenGLUsage(m_usage));
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pData, toOpenGLUsage(m_usage));
	}

}

void VertexBuffer::updateSubData(void* pData, size_t offset, size_t size)
{
	if (m_type == BufferType::Vertex) {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, pData);
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, pData);
	}

}

void VertexBuffer::setAttribute(int index, int size, int stride, size_t offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
}

OCF_END
