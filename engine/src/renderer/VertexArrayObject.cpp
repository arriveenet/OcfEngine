#include "ocf/renderer/VertexArrayObject.h"

#include "ocf/core/Engine.h"
#include "ocf/core/Macros.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/VertexBuffer2.h"
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

struct VertexArrayObject::BuilderDetails {
    PrimitiveType primitiveType = PrimitiveType::TRIANGLES;
    VertexBuffer* vertexBuffer = nullptr;
    IndexBuffer* indexBuffer = nullptr;
    size_t offset = 0;
    size_t count = 0;
};

VertexArrayObject::Builder::Builder()
{
    m_impl = new BuilderDetails();
}

VertexArrayObject::Builder::~Builder()
{
    delete m_impl;
}

VertexArrayObject::Builder& VertexArrayObject::Builder::geometry(PrimitiveType type,
                                                                 VertexBuffer* vb, IndexBuffer* ib,
                                                                 size_t offset, size_t count)
{
    OCF_ASSERT(vb != nullptr);

    m_impl->primitiveType = type;
    m_impl->vertexBuffer = vb;
    m_impl->indexBuffer = ib;
    m_impl->offset = offset;
    m_impl->count = count;
    return *this;
}

VertexArrayObject* VertexArrayObject::Builder::build(Engine& engine)
{
    VertexArrayObject* vao = new VertexArrayObject(engine, *this);
    return vao;
}

VertexArrayObject::VertexArrayObject(Engine& engine, const Builder& builder)
    : m_primitiveType(builder->primitiveType)
    , m_vertexBuffer(builder->vertexBuffer)
    , m_indexBuffer(builder->indexBuffer)
    , m_offset(builder->offset)
    , m_count(builder->count)
{
    Engine::Driver& driver = engine.getDriver();
    m_handle = driver.createRenderPrimitive(m_vertexBuffer->getHandle(),
                                            m_indexBuffer->getHandle(),
                                            m_primitiveType);
}

VertexArrayObject::~VertexArrayObject()
{
    terminate();
}

void VertexArrayObject::terminate()
{
}

} // namespace ocf
