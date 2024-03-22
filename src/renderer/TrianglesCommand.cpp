#include "TrianglesCommand.h"

OCF_BEGIN

TrianglesCommand::TrianglesCommand()
	: m_texture(nullptr)
{
	m_Type = Type::TrianglesCommand;
}

TrianglesCommand::~TrianglesCommand()
{
}

void TrianglesCommand::init(Texture2D* texture, const Triangles& triangles, const glm::mat4& modelView)
{
	RenderCommand::init(modelView);

	m_texture = texture;
	m_triangles = triangles;
}

OCF_END
