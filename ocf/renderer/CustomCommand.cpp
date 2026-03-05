#include "CustomCommand.h"

namespace ocf {
CustomCommand::CustomCommand()
    : m_pVertexArray(nullptr)
    , m_vertexDrawStart(0)
    , m_vertexDrawCount(0)
    , m_indexDrawOffset(0)
    , m_indexDrawCount(0)
    , m_textureCube(nullptr)
    , m_drawType(DrawType::Element)
    , m_primitiveType(PrimitiveType::TRIANGLES)
    , m_beforeCallback(nullptr)
    , m_afterCallback(nullptr) {
    m_Type = Type::CustomCommand;
    m_pVertexArray = new VertexArray();
}

CustomCommand::~CustomCommand()
{
    OCF_SAFE_DELETE(m_pVertexArray);
}

void CustomCommand::init(float globalZOrder, const glm::mat4& modelView)
{
    RenderCommand::init(globalZOrder, modelView);
}

} // namespace ocf
