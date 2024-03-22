#include "RenderCommand.h"

OCF_BEGIN

void RenderCommand::init(const glm::mat4& modelViewMatrix)
{
	m_modelVew = modelViewMatrix;
}

RenderCommand::RenderCommand()
	: m_Type(Type::UnknownCommand)
{
}

RenderCommand::~RenderCommand()
{
}

OCF_END
