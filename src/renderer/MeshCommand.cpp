#include "MeshCommand.h"

NS_OCF_BEGIN

MeshCommand::MeshCommand()
{
    m_Type = Type::MeshCommand;
    m_is3D = true;
}

MeshCommand::~MeshCommand()
{
}

void MeshCommand::init(float globalZOrder)
{
    RenderCommand::init(globalZOrder, glm::mat4(1.0f));
}

void MeshCommand::init(float globalZOrder, const glm::mat4& transform)
{
    RenderCommand::init(globalZOrder, transform);
}

NS_OCF_END
