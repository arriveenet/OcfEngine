#pragma once
#include "OpenGLInclude.h"
#include "base/Macros.h"

NS_OCF_BEGIN

enum class BufferType {
    Vertex,
    Index
};

enum class BufferUsage {
    Static,
    Dynamic
};

class VertexBuffer {
public:
    VertexBuffer(BufferType type, BufferUsage usage);
    ~VertexBuffer();

    GLuint getBuffer() const { return m_buffer; }
    BufferType getType() const { return m_type; }

    void bind() const;
    void unbind() const;

    void updateData(void* pData, size_t size);
    void updateSubData(void* pData, size_t offset, size_t size);

    void setAttribute(int index, int size, int stride, size_t offset);

private:
    GLuint m_buffer;
    BufferType m_type;
    BufferUsage m_usage;
};

NS_OCF_END
