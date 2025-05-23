#pragma once
#include "Types.h"
#include "RenderCommand.h"
#include <functional>

NS_OCF_BEGIN

class TextureCube;

class CustomCommand : public RenderCommand {
public:
    enum class DrawType {
        Array,
        Element
    };

    using CommandCallback = std::function<void()>;

    CustomCommand();
    ~CustomCommand();

    void init(float globalZOrder, const glm::mat4& modelView);

    VertexArray* getVertexArray() const { return m_pVertexArray; }

    void setDrawType(DrawType drawType) { m_drawType = drawType; }
    DrawType getDrawType() const { return m_drawType; }

    void setPrimitiveType(PrimitiveType primitiveType) { m_primitiveType = primitiveType; }
    PrimitiveType getPrimitiveType() const { return m_primitiveType; }

    void setVertexDrawInfo(unsigned int start, unsigned int count)
    {
        m_vertexDrawStart = start;
        m_vertexDrawCount = count;
    }
    unsigned int getVertexDrawCount() const { return m_vertexDrawCount; }
    unsigned int getVertexDrawStart() const { return m_vertexDrawStart; }

    void setIndexDrawInfo(unsigned int offset, unsigned int count)
    {
        m_indexDrawOffset = offset;
        m_indexDrawCount = count;
    }
    unsigned int getIndexDrawCount() const { return m_indexDrawCount; }
    unsigned int getIndexDrawOffset() const { return m_indexDrawOffset; }

    void setTextureCube(TextureCube* textureCube) { m_textureCube = textureCube; }
    TextureCube* getTextureCube() const { return m_textureCube; }

    void setBeforeCallback(CommandCallback callback) { m_beforeCallback = callback; }
    const CommandCallback &getBeforeCallback() const { return m_beforeCallback; }

    void setAfterCallback(CommandCallback callback) { m_afterCallback = callback; }
    const CommandCallback &getAfterCallback() const { return m_afterCallback; }

protected:
    VertexArray* m_pVertexArray;
    unsigned int m_vertexDrawStart;
    unsigned int m_vertexDrawCount;
    unsigned int m_indexDrawOffset;
    unsigned int m_indexDrawCount;

    TextureCube* m_textureCube;

    DrawType m_drawType;
    PrimitiveType m_primitiveType;

    CommandCallback m_beforeCallback;
    CommandCallback m_afterCallback;
};

NS_OCF_END
