#pragma once
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/renderer/backend/Handle.h"

namespace ocf {

class VertexBuffer;
class IndexBuffer;
class Engine;

class VertexArrayObject {
    struct BuilderDetails;

public:
    using RenderPrimitiveHandle = backend::RenderPrimitiveHandle;
    using PrimitiveType = backend::PrimitiveType;

    class Builder {
    public:
        Builder();
        ~Builder();

        BuilderDetails* operator->() noexcept
        {
            return m_impl;
        }
        const BuilderDetails* operator->() const noexcept
        {
            return m_impl;
        }

        Builder& geometry(PrimitiveType type, VertexBuffer* vb,
                          IndexBuffer* ib, size_t offset, size_t count);

        VertexArrayObject* build(Engine& engine);

    private:
        BuilderDetails* m_impl = nullptr;
    };

    VertexArrayObject(Engine& engine, const Builder& builder);
    ~VertexArrayObject();

    void terminate();

    RenderPrimitiveHandle getHandle() const noexcept { return m_handle; }

    PrimitiveType getPrimitiveType() const noexcept { return m_primitiveType; }

    bool hasIndexBuffer() const noexcept { return m_indexBuffer != nullptr; }

    size_t getVertexCount() const noexcept { return m_count; }

    size_t getVertexOffset() const noexcept { return m_offset; }

    size_t offset() const noexcept { return m_offset; }

protected:
    RenderPrimitiveHandle m_handle;
    PrimitiveType m_primitiveType = PrimitiveType::TRIANGLES;
    size_t m_offset = 0;
    size_t m_count = 0;
    VertexBuffer* m_vertexBuffer = nullptr;
    IndexBuffer* m_indexBuffer = nullptr;
};

} // namespace ocf
