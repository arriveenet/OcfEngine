#include "ocf/renderer/IndexBuffer.h"

#include "renderer/backend/DriverBase.h"
#include "ocf/core/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include <iostream>

namespace ocf {

using namespace backend;

struct IndexBuffer::BuilderDetails {
    uint32_t indexCount = 0;
    IndexBuffer::IndexType indexType = IndexBuffer::IndexType::USHORT;
};

IndexBuffer::Builder::Builder()
{
    m_impl = new BuilderDetails();
}

IndexBuffer::Builder::~Builder()
{
    delete m_impl;
}

IndexBuffer::Builder& IndexBuffer::Builder::indexCount(uint32_t indexCount) noexcept
{
    m_impl->indexCount = indexCount;
    return *this;
}

IndexBuffer* IndexBuffer::Builder::build(Engine& engine)
{
    IndexBuffer* indexBuffer = new IndexBuffer(engine, *this);
    return indexBuffer;
}

IndexBuffer::IndexBuffer(Engine& engine, const Builder& builder)
    : m_indexCount(builder->indexCount)
    , m_indexType(builder->indexType)
{
    const size_t bufferSize =
        m_indexCount * (m_indexType == IndexType::USHORT ? sizeof(uint16_t) : sizeof(uint32_t));

    Engine::Driver& driver = engine.getDriver();
    m_handle = driver.createIndexBuffer(static_cast<ElementType>(m_indexType), m_indexCount,
                                        BufferUsage::STATIC);
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::setBufferData(Engine& engine, const void* data, size_t size,
                                size_t offset) noexcept
{
    engine.getDriver().updateIndexBufferData(m_handle, data, size, offset);
}

} // namespace ocf
