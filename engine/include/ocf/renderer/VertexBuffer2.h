#pragma once
#include "ocf/renderer/MaterialEnums.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/renderer/backend/Handle.h"

namespace ocf {

class Engine;

class VertexBuffer {
    struct BuilderDetails;

public:
    using VertexBufferInfoHandle = backend::VertexBufferInfoHandle;
    using VertexBufferHandle = backend::VertexBufferHandle;
    using AttributeType = backend::ElementType;
    using BufferUsage = backend::BufferUsage;

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

        Builder& bufferCount(uint8_t bufferCount) noexcept;
        Builder& vertexCount(uint32_t vertexCount) noexcept;
        Builder& attribute(VertexAttribute attribute, AttributeType type, uint8_t stride,
                           uint32_t offset) noexcept;

        VertexBuffer* build(Engine& engine);

    private:
        BuilderDetails* m_impl = nullptr;
    };

    VertexBuffer(Engine& engine, const Builder& builder);
    ~VertexBuffer();

    VertexBufferHandle getHandle() const noexcept
    {
        return m_handle;
    }

    VertexBufferInfoHandle getVertexBufferInfoHandle() const noexcept
    {
        return m_vertexBufferInfoHandle;
    }

    uint32_t getVertexCount() const noexcept
    {
        return m_vertexCount;
    }

    void setBufferData(Engine& engine, const void* data, size_t size, size_t offset) const;

private:
    VertexBufferHandle m_handle;
    VertexBufferInfoHandle m_vertexBufferInfoHandle;
    backend::AttributeArray m_attributes;
    uint32_t m_vertexCount;
    uint32_t m_byteCount;
    BufferUsage m_usage;
};

} // namespace ocf
