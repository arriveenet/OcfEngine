#include "DrawShape.h"
#include "base/Game.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderManager.h"
#include <glm/gtc/constants.hpp>

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
    : m_dirtyPoint(false)
    , m_dirtyLine(false)
    , m_dirtyTriangle(false)
    , m_bufferCapacityPoint(0)
    , m_bufferCountPoint(0)
    , m_bufferCapacityLine(0)
    , m_bufferCountLine(0)
    , m_bufferCapacityTriangle(0)
    , m_bufferCountTriangle(0)
    , m_pointSize(1.0f)
    , m_lineWidth(1.0f)
{
    setName("DrawShape");
}

DrawShape::~DrawShape()
{
}

bool DrawShape::init()
{
    updateShader(m_customCommandPoint, ProgramType::DrawShape, PrimitiveType::Point);
    updateShader(m_customCommandLine, ProgramType::DrawShape, PrimitiveType::Line);
    updateShader(m_customCommandTriangle, ProgramType::DrawShape, PrimitiveType::Triangle);

    m_customCommandPoint.setBeforeCallback(
        [=]() { m_pGame->getRenderer()->setPointSize(m_pointSize);});

    m_customCommandLine.setBeforeCallback(
        [=]() { m_pGame->getRenderer()->setLineWidth(m_lineWidth); });

    return true;
}

void DrawShape::clear()
{
    m_bufferCountLine = 0;
    m_dirtyLine = true;

    m_bufferCountTriangle = 0;
    m_dirtyTriangle = true;
}

void DrawShape::ensureCapacityGLPoint(int count)
{
    if (m_bufferCountPoint + count > m_bufferCapacityPoint) {
        m_bufferCapacityPoint += std::max<int>(m_bufferCapacityPoint, count);
        m_pointBuffers.resize(m_bufferCapacityPoint);
        VertexArray *pVertexArray = m_customCommandPoint.getVertexArray();
        pVertexArray->updateVertexBuffer(m_pointBuffers.data(),
                                         sizeof(Vertex3fC4f) * m_bufferCapacityPoint);
    }
}

void DrawShape::ensureCapacityGLLine(int count)
{
    if (m_bufferCountLine + count > m_bufferCapacityLine) {
        m_bufferCapacityLine += std::max<int>(m_bufferCapacityLine, count);
        m_lineBuffers.resize(m_bufferCapacityLine);

        VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
        pVertexArray->updateVertexBuffer(m_lineBuffers.data(),
                                         sizeof(Vertex3fC4f) * m_bufferCapacityLine);
    }
}

void DrawShape::ensureCapacityGLTriangle(int count)
{
    if (m_bufferCountTriangle + count > m_bufferCapacityTriangle) {
        m_bufferCapacityTriangle += std::max<int>(m_bufferCapacityTriangle, count);
        m_triangleBuffers.resize(m_bufferCapacityTriangle);
        VertexArray* pVertexArray = m_customCommandTriangle.getVertexArray();
        pVertexArray->updateVertexBuffer(m_triangleBuffers.data(),
                                         sizeof(Vertex3fC4f) * m_bufferCapacityTriangle);
    }
}

void DrawShape::drawPoint(const glm::vec2 &point, const glm::vec4 &color)
{
    ensureCapacityGLPoint(1);
    m_pointBuffers[m_bufferCountPoint] = {glm::vec3(point, 0.0f), color};
    VertexArray *pVertexArray = m_customCommandPoint.getVertexArray();
    pVertexArray->updateVertexBuffer(m_pointBuffers.data() + m_bufferCountPoint,
                                     sizeof(Vertex3fC4f) * m_bufferCountPoint,
                                     sizeof(Vertex3fC4f) * 1);
    m_bufferCountPoint += 1;
    m_dirtyPoint = true;
    m_customCommandPoint.setVertexDrawInfo(0, m_bufferCountPoint);
}

void DrawShape::drawLine(const glm::vec2 &origin, const glm::vec2 &destanation,
                         const glm::vec4 &color)
{
    drawLine(glm::vec3(origin, 0.0f), glm::vec3(destanation, 0.0f), color);
}

void DrawShape::drawLine(const glm::vec3& origin, const glm::vec3& destanation, const glm::vec4& color)
{
    ensureCapacityGLLine(2);

    m_lineBuffers[m_bufferCountLine] = { origin, color };
    m_lineBuffers[m_bufferCountLine + 1] = { destanation, color };

    VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
    pVertexArray->updateVertexBuffer(m_lineBuffers.data() + m_bufferCountLine,
                                     sizeof(Vertex3fC4f) * m_bufferCountLine,
                                     sizeof(Vertex3fC4f) * 2);


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

void DrawShape::drawFillRect(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color)
{
    std::vector<glm::vec2> vertices = {
        origin,
        glm::vec2(destanation.x, origin.y),
        destanation,
        glm::vec2(origin.x, destanation.y)
    };

    drawPolygon(vertices, color);
}

void DrawShape::drawFillTriangle(const glm::vec2 &a, const glm::vec2 &b,
                                 const glm::vec2 &c, const glm::vec4 &color)
{
  std::vector<glm::vec2> vertices = {a, b, c};

   drawPolygon(vertices, color);
}

void DrawShape::drawFillCircle(const glm::vec2& center, float radius, const glm::vec4& color)
{
    constexpr int segments = 36; // 円を描くためのセグメント数
    constexpr float angleIncrement = 2.0f * glm::pi<float>() / segments;

    std::vector<glm::vec2> vertices;
    vertices.reserve(segments + 1); // セグメント数 + 中心点
    vertices.push_back(center);     // 中心点を追加

    for (int i = 0; i <= segments; ++i) {
        float angle = angleIncrement * static_cast<float>(i);
        float x = center.x + radius * cosf(angle);
        float y = center.y + radius * sinf(angle);
        vertices.emplace_back(x, y);
    }

    // 最後の頂点は最初の頂点と同じにする
    vertices.push_back(vertices[1]);

    drawPolygon(vertices, color);
}

void DrawShape::drawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color)
{
    auto triangles = triangulate(vertices);

    ensureCapacityGLTriangle(static_cast<int>(triangles.size()));

    for (size_t i = 0; i < triangles.size(); ++i) {
        m_triangleBuffers[m_bufferCountTriangle + i] = { glm::vec3(triangles[i], 0.0f), color };
    }

    VertexArray* pVertexArray = m_customCommandTriangle.getVertexArray();
    pVertexArray->updateVertexBuffer(m_triangleBuffers.data() + m_bufferCountTriangle,
                                     sizeof(Vertex3fC4f) * m_bufferCountTriangle,
                                     sizeof(Vertex3fC4f) * static_cast<int>(triangles.size()));

    m_bufferCountTriangle += static_cast<int>(triangles.size());
    m_dirtyTriangle = true;

    m_customCommandTriangle.setVertexDrawInfo(0, m_bufferCountTriangle);

    // Debug print
    //for (size_t i = 0; i < triangles.size(); ++i) {
    //    if (i % 3 == 0) {
    //        OCFLOG("======== Triangle %zu ========\n", i / 3);
    //    }
    //    const auto& triangle = triangles[i];
    //    OCFLOG("Vertex: (%.2f, %.2f)\n", triangle.x, triangle.y);
    //}
}

void DrawShape::drawPolyline(const std::vector<glm::vec2>& /*vertices*/, const glm::vec4& /*color*/)
{

}

void DrawShape::draw(Renderer* renderer, const glm::mat4& transform)
{
    if (m_bufferCountPoint > 0) {
        updateUniforms(transform, m_customCommandPoint);
        m_customCommandPoint.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandPoint);
    }   

    if (m_bufferCountLine > 0) {
        updateUniforms(transform, m_customCommandLine);
        m_customCommandLine.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandLine);
    }

    if (m_bufferCountTriangle > 0) {
        updateUniforms(transform, m_customCommandTriangle);
        m_customCommandTriangle.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandTriangle);
    }
}

void DrawShape::updateShader(CustomCommand& command, ProgramType programType, PrimitiveType primitiveType)
{
    command.setDrawType(CustomCommand::DrawType::Array);
    command.setPrimitiveType(primitiveType);

    Program* pProgram = ShaderManager::getInstance()->getBuiltinProgram(programType);

    auto& programState = command.getProgramState();
    programState.setProgram(pProgram);

    VertexArray* pVertexArray = command.getVertexArray();
    pVertexArray->bind();

    pVertexArray->setStride(sizeof(Vertex3fC4f));

    pVertexArray->createVertexBuffer(BufferUsage::Dynamic);
    pVertexArray->updateVertexBuffer(nullptr, 0);

    pVertexArray->setAttribute("inPosition", 0, 3, false, 0);
    pVertexArray->setAttribute("inColor", 1, 4, false, sizeof(float) * 3);

    pVertexArray->bindVertexBuffer();

    pVertexArray->unbind();
}

void DrawShape::updateUniforms(const glm::mat4& transform, CustomCommand& cmd)
{
    glm::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
    auto& programState = cmd.getProgramState();

    programState.setUniform("uProjection", &projection, sizeof(projection));
    programState.setUniform("uModelView", &transform, sizeof(transform));
}

bool DrawShape::isConvex(const glm::vec2& prev, const glm::vec2& curr, const glm::vec2& next)
{
    auto edge1 = curr - prev;
    auto edge2 = next - curr;

    // 外積で凸性を判定
    return (edge1.x * edge2.y - edge1.y * edge2.x) > 0;
}

bool DrawShape::isPointInTriangle(const glm::vec2& p,
                                  const glm::vec2& a,
                                  const glm::vec2& b,
                                  const glm::vec2& c)
{
    auto sign = [](const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
        };
    float d1 = sign(p, a, b);
    float d2 = sign(p, b, c);
    float d3 = sign(p, c, a);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

std::vector<glm::vec2> DrawShape::triangulate(const std::vector<glm::vec2>& vertices)
{
    OCFASSERT(vertices.size() >= 3, "Triangulation requires at least 3 vertices");

    std::vector<glm::vec2> triangles;
    std::vector<glm::vec2> remainingVertices = vertices;

    while (remainingVertices.size() > 3) {
        bool foundTriangle = false;

        for (size_t i = 0; i < remainingVertices.size(); ++i) {
            glm::vec2 prev = remainingVertices[(i - 1 + remainingVertices.size()) % remainingVertices.size()];
            glm::vec2 curr = remainingVertices[i];
            glm::vec2 next = remainingVertices[(i + 1) % remainingVertices.size()];

            // 頂点が凸であるかを確認
            if (isConvex(prev, curr, next)) {
                bool isEar = true;

                // 三角形の内部に他の頂点があるかを確認
                for (const auto& vertex : remainingVertices) {
                    if (vertex != prev && vertex != curr && vertex != next) {
                        if (isPointInTriangle(vertex, prev, curr, next)) {
                            isEar = false;
                            break;
                        }
                    }
                }

                if (isEar) {
                    triangles.emplace_back(prev);
                    triangles.emplace_back(curr);
                    triangles.emplace_back(next);
                    remainingVertices.erase(remainingVertices.begin() + i);
                    foundTriangle = true;
                    break;
                }
            }

            if (foundTriangle) {
                OCFLOG("Triangulation failed: no ear found");
                break;
            }
        }
    }

    // 最後の3つの頂点を三角形として追加
    if (remainingVertices.size() == 3) {
        triangles.emplace_back(remainingVertices[0]);
        triangles.emplace_back(remainingVertices[1]);
        triangles.emplace_back(remainingVertices[2]);
    }

    return triangles;
}

NS_OCF_END
