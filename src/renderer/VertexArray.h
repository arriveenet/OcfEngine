#pragma once
#include "OpenGLInclude.h"
#include "base/Macros.h"
#include "base/Types.h"

OCF_BEGIN

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void init(Vertex3fC3fT2f* vertices, unsigned short* indices, unsigned int vertexCount, unsigned int indexCount);

	unsigned int getVertexCount() const { return m_vertexCount; }
	unsigned int getIndexCount() const { return m_indexCount; }

	GLuint getHandle() const { return m_vertexArray; }

	void bind();
	void unbind();

private:
	GLuint m_vertexArray;

	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;

	unsigned int m_vertexCount;
	unsigned int m_indexCount;
};

OCF_END
