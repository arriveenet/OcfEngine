#pragma once
#include "Types.h"
#include "RenderCommand.h"

OCF_BEGIN

class CustomCommand : public RenderCommand {
public:
	enum class DrawType {
		Array,
		Element
	};

	CustomCommand();
	~CustomCommand();

	void init(const glm::mat4& modelView);

	void createVertexBuffer(BufferUsage usage);
	void createIndexBuffer(BufferUsage usage);

	void updateVertexBuffer(void* pData, size_t size);
	void updateIndexBuffer(void* pData, size_t size);

	VertexArray* getVertexArray() const { return m_pVertexArray; }
	VertexBuffer* getVertexBuffer() const { return m_pVertexBuffer; }
	VertexBuffer* getIndexBuffer() const { return m_pIndexBuffer; }

	void setDrawType(DrawType drawType) { m_drawType = drawType; }
	DrawType getDrawType() const { return m_drawType; }

	void setPrimitiveType(PrimitiveType primitiveType) { m_primitiveType = primitiveType; }
	PrimitiveType getPrimitiveType() const { return m_primitiveType; }

protected:
	VertexArray* m_pVertexArray;
	VertexBuffer* m_pVertexBuffer;
	VertexBuffer* m_pIndexBuffer;

	DrawType m_drawType;
	PrimitiveType m_primitiveType;
};

OCF_END
