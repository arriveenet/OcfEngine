#include "RenderCommand.h"

NS_OCF_BEGIN

void RenderCommand::init(const glm::mat4& modelViewMatrix)
{
	m_modelVew = modelViewMatrix;
}

RenderCommand::RenderCommand()
	: m_Type(Type::UnknownCommand)
	, m_globalOrder(0.0f)
	, m_modelVew(1.0f)
{
}

RenderCommand::~RenderCommand()
{
}

NS_OCF_END
