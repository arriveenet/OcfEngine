#include "RenderCommand.h"

OCF_BEGIN

void RenderCommand::init(const glm::mat4& modelViewMatrix)
{
	m_modelVew = modelViewMatrix;
}

RenderCommand::RenderCommand()
{
}

RenderCommand::~RenderCommand()
{
}

OCF_END
