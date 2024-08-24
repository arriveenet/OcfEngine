#pragma once
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Program.h"
#include "ProgramState.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "base/Types.h"

NS_OCF_BEGIN

class RenderCommand {
public:
    enum class Type {
        UnknownCommand,
        TrianglesCommand,
        CustomCommand,
        MeshCommand
    };

    void init(float globalZOrder, const glm::mat4& modelViewMatrix);

    float getGlobalOrder() const { return m_globalOrder; }

    Type getType() const { return m_Type; }

    bool is3D() const { return m_is3D; }
    void set3D(bool value) { m_is3D = value; }

    const glm::mat4& getModelView() const { return m_modelVew; }
    ProgramState& getProgramState() { return m_programState; }

protected:
    RenderCommand();
    virtual ~RenderCommand();

    Type m_Type;
    bool m_is3D;
    float m_globalOrder;
    glm::mat4 m_modelVew;
    ProgramState m_programState;
};


NS_OCF_END
