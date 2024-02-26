#pragma once
#include "OpenGLInclude.h"
#include "base/Macros.h"

OCF_BEGIN

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	GLuint getArray() const { return m_array; }

	void setAttribute(int index, int size, int stride, size_t offset);

private:
	GLuint m_array;
};

OCF_END
