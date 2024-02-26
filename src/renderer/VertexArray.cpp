#include "VertexArray.h"

OCF_BEGIN

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_array);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_array);
}

void VertexArray::setAttribute(int index, int size, int stride, size_t offset)
{
	glEnableVertexAttribArray(index);

	glVertexAttribPointer(index, size, GL_FLOAT, FALSE, stride, (void*)offset);
}


OCF_END
