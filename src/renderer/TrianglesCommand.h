#pragma once
#include "RenderCommand.h"

OCF_BEGIN

class TrianglesCommand : public RenderCommand {
public:
	struct Triangles {
		Triangles(Vertex3fC3fT2f* _vertices, unsigned short* _indices, unsigned int _vertexCount, unsigned int _indexCount)
			: vertices(_vertices)
			, indices(_indices)
			, vertexCount(_vertexCount)
			, indexCount(_indexCount)
		{
		}

		Triangles() {}

		Vertex3fC3fT2f* vertices = nullptr;
		unsigned short* indices = nullptr;
		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
	};

	TrianglesCommand();
	~TrianglesCommand();

	void init(Texture2D* texture, const Triangles& triangles, const glm::mat4& modelView);

	const Triangles& getTriangles() const { return m_triangles; }
	Texture2D* getTexture() const { return m_texture; }

protected:
	Triangles m_triangles;
	Texture2D* m_texture;

	GLuint m_vertexArray;
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;
};

OCF_END
