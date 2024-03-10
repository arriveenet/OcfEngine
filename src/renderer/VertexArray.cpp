#include "VertexArray.h"

OCF_BEGIN

VertexArray::VertexArray()
	: m_vertexArray(0)
	, m_vertexCount(0)
	, m_indexCount(0)
	, m_vertexBuffer(BufferType::Vertex, BufferUsage::Static)
	, m_indexBuffer(BufferType::Index, BufferUsage::Static)
	, m_stride(0)
{
	glGenVertexArrays(1, &m_vertexArray);
}

VertexArray::~VertexArray()
{

}

void VertexArray::init(Vertex3fC3fT2f* vertices, unsigned short* indices, unsigned int vertexCount, unsigned int indexCount)
{
	// Create vertex array
	bind();

	// Create vertex buffer
	m_vertexBuffer.updateData(vertices, sizeof(Vertex3fC3fT2f) * vertexCount);

	// Create index buffer
	m_indexBuffer.updateData(indices, sizeof(unsigned short) * indexCount);

	// Specify the vertex attributes
	setStride(sizeof(Vertex3fC3fT2f));
	setAttribute("inPosition", 0, 3, false, 0);
	setAttribute("inNormal", 1, 3, false, sizeof(float) * 3);
	setAttribute("inTexCoord", 2, 2, false, sizeof(float) * 6);

	bindVertexBuffer();

	unbind();
}

void VertexArray::bind()
{
	glBindVertexArray(m_vertexArray);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::setStride(std::size_t stride)
{
	m_stride = stride;
}

void VertexArray::updateVertexData(void* pData, size_t size)
{
	m_vertexBuffer.updateData(pData, size);
}

void VertexArray::updateIndexData(void* pData, size_t size)
{
	m_indexBuffer.updateData(pData, size);
}

void VertexArray::bindVertexBuffer()
{
	for (const auto& attributeInfo : m_attributes) {
		const auto& attribute = attributeInfo.second;

		m_indexBuffer.setAttribute(attribute.index, attribute.size, static_cast<int>(m_stride), attribute.offset);
	}
}

void VertexArray::setAttribute(const std::string& name, int index, int size, bool needToBeNormallized, std::size_t offset)
{
	Attribute attrib(name, index, size, needToBeNormallized, offset);
	m_attributes.emplace(name, attrib);
}

OCF_END
