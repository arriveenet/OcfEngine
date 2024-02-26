#pragma once
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "base/Types.h"

OCF_BEGIN

class RenderCommand {
public:
	void init(const glm::mat4& modelViewMatrix);

	const glm::mat4& getModelView() const { return m_modelVew; }

protected:
	RenderCommand();
	virtual ~RenderCommand();

	glm::mat4 m_modelVew;
};


class TrianglesCommand : public RenderCommand {
public:
	struct Triangles {
		Triangles(Vertex3fT2f* _vertices, unsigned short* _indices, unsigned int _vertexCount, unsigned int _indexCount)
			: vertices(_vertices)
			, indices(_indices)
			, vertexCount(_vertexCount)
			, indexCount(_indexCount)
		{
		}

		Triangles() {}

		Vertex3fT2f* vertices = nullptr;
		unsigned short* indices = nullptr;
		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
	};

	TrianglesCommand();
	~TrianglesCommand();

	void init(Texture2D* texture, Program* program, const Triangles& triangles, const glm::mat4& modelView);

	void draw();

	const Triangles& getTriangles() const { return m_triangles; }

protected:
	Triangles m_triangles;
	Texture2D* m_texture;
	Program* m_program;
	VertexBuffer* m_vertexBuffer;
	VertexArray* m_vertexArray;
};

OCF_END
