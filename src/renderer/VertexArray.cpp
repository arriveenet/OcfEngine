#include "VertexArray.h"

OCF_BEGIN

VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{

}

void VertexArray::init(Vertex3fC3fT2f* vertices, unsigned short* indices, unsigned int vertexCount, unsigned int indexCount)
{
	// Create vertex array
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	// Create vertex buffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex3fC3fT2f), vertices, GL_STATIC_DRAW);

	// Create index buffer
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	// Specify the vertex attributes
	// (For now, assume one vertex format)
	// Position is 3 floats
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	// Normal is 3 floats
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 3));
	// Texture coordinates is 2 floats
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 6));
}

void VertexArray::setAttribute(int index, int size, int stride, size_t offset)
{

}


OCF_END
