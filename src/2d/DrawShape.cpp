#include "DrawShape.h"
#include "base/Game.h"
#include "renderer/ShaderManager.h"

NS_OCF_BEGIN

DrawShape* DrawShape::create()
{
    DrawShape* drawShape = new DrawShape();
    if (drawShape->init()) {
        return drawShape;
    }

    return nullptr;
}

DrawShape::DrawShape()
    : m_dirtyLine(false)
    , m_bufferCapacityLine(0)
    , m_bufferCountLine(0)
{
}

DrawShape::~DrawShape()
{
}

bool DrawShape::init()
{
    m_customCommandLine.setDrawType(CustomCommand::DrawType::Array);
    m_customCommandLine.setPrimitiveType(PrimitiveType::Line);

    Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::DrawShape);

    auto& programState = m_customCommandLine.getProgramState();
    programState.setProgram(pProgram);

    VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
    pVertexArray->bind();

    pVertexArray->setStride(sizeof(Vertex3fC4f));

    pVertexArray->createVertexBuffer(BufferUsage::Dynamic);
    ensureCapacityGLLine(256);

    pVertexArray->setAttribute("inPosition", 0, 3, false, 0);
    pVertexArray->setAttribute("inColor", 1, 4, false, sizeof(float) * 3);

    pVertexArray->bindVertexBuffer();

    pVertexArray->unbind();

    return true;
}

void DrawShape::clear()
{
    m_bufferCountLine = 0;
    m_dirtyLine = true;
}

void DrawShape::ensureCapacityGLLine(int count)
{
    if (m_bufferCountLine + count > m_bufferCapacityLine) {
        m_bufferCapacityLine += std::max<int>(m_bufferCapacityLine, count);
        m_lineBuffers.resize(m_bufferCapacityLine);

        VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
        pVertexArray->updateVertexBuffer(m_lineBuffers.data(), sizeof(Vertex3fC4f) * m_bufferCapacityLine);
    }
}

void DrawShape::drawLine(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color)
{
    drawLine(glm::vec3(origin, 0.0f), glm::vec3(destanation, 0.0f), color);
}

void DrawShape::drawLine(const glm::vec3& origin, const glm::vec3& destanation, const glm::vec4& color)
{
    ensureCapacityGLLine(2);

    m_lineBuffers[m_bufferCountLine] = { origin, color };
    m_lineBuffers[m_bufferCountLine + 1] = { destanation, color };

    VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
    pVertexArray->updateVertexBuffer(m_lineBuffers.data() + m_bufferCountLine, sizeof(Vertex3fC4f) * m_bufferCountLine, sizeof(Vertex3fC4f) * 2);


    m_bufferCountLine += 2;
    m_dirtyLine = true;

    m_customCommandLine.setVertexDrawInfo(0, m_bufferCountLine);
}

void DrawShape::drawRect(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color)
{
    drawLine(origin, glm::vec2(destanation.x, origin.y), color);
    drawLine(glm::vec2(destanation.x, origin.y), destanation, color);
    drawLine(destanation, glm::vec2(origin.x, destanation.y), color);
    drawLine(glm::vec2(origin.x, destanation.y), origin, color);
}

void DrawShape::update(float deltaTime)
{
}

void DrawShape::draw(Renderer* renderer, const glm::mat4& transform)
{
    if (m_bufferCountLine > 0) {
        updateUniforms(transform, m_customCommandLine);
        m_customCommandLine.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandLine);
    }

}

void DrawShape::updateVertexBuffer()
{
}

void DrawShape::updateUniforms(const glm::mat4& transform, CustomCommand& cmd)
{
    glm::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
    auto& programState = cmd.getProgramState();

    programState.setUniform("uProjection", projection);
    programState.setUniform("uModelView", transform);
}

NS_OCF_END
