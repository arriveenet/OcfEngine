#include "QuadCommand.h"

OCF_BEGIN

QuadCommand::QuadCommand()
{
}

QuadCommand::~QuadCommand()
{
}

void QuadCommand::init(Texture2D* texture, QuadV3fC3fT2f* pQuads, unsigned short* pIndices, size_t quadCount, const glm::mat4& modelView)
{
	Triangles triangles;
	triangles.vertices = &pQuads->topLeft;
	triangles.vertexCount = static_cast<unsigned int>(quadCount * 4);
	triangles.indices = pIndices;
	triangles.indexCount = static_cast<unsigned int>(quadCount * 6);

	TrianglesCommand::init(texture, triangles, modelView);
}

OCF_END
