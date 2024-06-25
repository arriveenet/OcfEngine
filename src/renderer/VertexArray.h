#pragma once
#include <string>
#include <unordered_map>
#include "OpenGLInclude.h"
#include "VertexBuffer.h"
#include "base/Macros.h"
#include "base/Types.h"

NS_OCF_BEGIN

class VertexArray {
public:
	struct Attribute {
		Attribute(std::string _name, int _index, int _size, bool _needToBeNormallized, std::size_t _offset)
			: name(_name), index(_index), size(_size), needToBeNormallized(_needToBeNormallized), offset(_offset)
		{
		}

		std::string name;
		int index = 0;
		int size = 3;
		bool needToBeNormallized = false;
		std::size_t offset = 0;
	};

	VertexArray();
	~VertexArray();

	void init(Vertex3fC3fT2f* vertices, unsigned short* indices, unsigned int vertexCount, unsigned int indexCount);

	unsigned int getVertexCount() const { return m_vertexCount; }
	unsigned int getIndexCount() const { return m_indexCount; }

	GLuint getHandle() const { return m_vertexArray; }

	void bind();
	void unbind();

	std::size_t getStride() const { return m_stride; }
	void setStride(std::size_t stride);

	void createVertexBuffer(BufferUsage usage);
	void createIndexBuffer(BufferUsage usage);

	void updateVertexBuffer(void* pData, size_t size);
	void updateIndexBuffer(void* pData, size_t size);
	void updateVertexBuffer(void* pData, size_t offset, size_t size);
	void updateIndexBuffer(void* pData, size_t offset, size_t size);

	void bindVertexBuffer();
	void setAttribute(const std::string& name, int index, int size, bool needToBeNormallized, std::size_t offset);

private:
	GLuint m_vertexArray;

	VertexBuffer* m_vertexBuffer;
	VertexBuffer* m_indexBuffer;

	unsigned int m_vertexCount;
	unsigned int m_indexCount;

	std::size_t m_stride;
	std::unordered_map<std::string, Attribute> m_attributes;
};

NS_OCF_END
