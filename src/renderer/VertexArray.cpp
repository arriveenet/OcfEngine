#include "VertexArray.h"

NS_OCF_BEGIN

VertexArray::VertexArray()
    : m_vertexArray(0)
    , m_vertexCount(0)
    , m_indexCount(0)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_isVertexBufferCreated(false)
    , m_isIndexBufferCreated(false)
    , m_stride(0)
{
    glGenVertexArrays(1, &m_vertexArray);
}

VertexArray::~VertexArray()
{
    OCF_SAFE_DELETE(m_vertexBuffer);
    OCF_SAFE_DELETE(m_indexBuffer);
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

void VertexArray::createVertexBuffer(BufferUsage usage)
{
    OCF_SAFE_DELETE(m_vertexBuffer);

    m_isVertexBufferCreated = true;

    m_vertexBuffer = new VertexBuffer(BufferType::Vertex, usage);
}

void VertexArray::createIndexBuffer(BufferUsage usage)
{
    OCF_SAFE_DELETE(m_indexBuffer);

    m_isIndexBufferCreated = true;

    m_indexBuffer = new VertexBuffer(BufferType::Index, usage);
}

void VertexArray::updateVertexBuffer(void* pData, size_t size)
{
    m_vertexCount = static_cast<unsigned int>(size);

    m_vertexBuffer->updateData(pData, size);
}

void VertexArray::updateIndexBuffer(void* pData, size_t size)
{
    m_indexCount = static_cast<unsigned int>(size);

    m_indexBuffer->updateData(pData, size);
}

void VertexArray::updateVertexBuffer(void* pData, size_t offset, size_t size)
{
    m_vertexBuffer->updateSubData(pData, offset, size);
}

void VertexArray::updateIndexBuffer(void* pData, size_t offset, size_t size)
{
    m_indexBuffer->updateSubData(pData, offset, size);
}

void VertexArray::bindVertexBuffer()
{
    OCFASSERT(m_isVertexBufferCreated, "Vertex buffer not created");
    OCFASSERT(m_stride != 0, "not set stride");

    for (const auto& attributeInfo : m_attributes) {
        const auto& attribute = attributeInfo.second;

        m_indexBuffer->setAttribute(attribute.index, attribute.size, static_cast<int>(m_stride), attribute.offset);
    }
}

void VertexArray::setAttribute(const std::string& name, int index, int size, bool needToBeNormallized, std::size_t offset)
{
    Attribute attrib(name, index, size, needToBeNormallized, offset);
    m_attributes.emplace(name, attrib);
}

NS_OCF_END
