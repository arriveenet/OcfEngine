#pragma once
#include "OpenGLInclude.h"
#include "base/Macros.h"

OCF_BEGIN

enum class BufferType {
	Vertex,
	Index
};

enum class BufferUsage {
	Static,
	Dynamic
};

class VertexBuffer {
public:
	VertexBuffer(BufferType type, BufferUsage usage);
	~VertexBuffer();

	GLuint getBuffer() const { return m_buffer; }

	void updateData(void* pData, size_t size);

private:
	GLuint m_buffer;
	BufferType m_type;
	BufferUsage m_usage;
};

OCF_END
