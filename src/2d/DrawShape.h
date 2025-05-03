#pragma once

#include "2d/Node.h"
#include "renderer/VertexArray.h"
#include "renderer/CustomCommand.h"
#include "renderer/ShaderManager.h"
#include <vector>

NS_OCF_BEGIN

class DrawShape : public Node {
public:
    static DrawShape* create();

    DrawShape();
    virtual ~DrawShape();

    bool init() override;
    void clear();

    void ensureCapacityGLLine(int count);
    void ensureCapacityGLTriangle(int count);

    void drawLine(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);
    void drawLine(const glm::vec3& origin, const glm::vec3& destanation, const glm::vec4& color);
    void drawRect(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);
    void drawFilledRect(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);
    void drawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color);

    void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
    void updateShader(CustomCommand& cmd, ProgramType programType, PrimitiveType primitiveType);
    void updateUniforms(const glm::mat4& transform, CustomCommand& cmd);
    /**
      * @brief 凸ポリゴンかどうかを判定する
      * @param prev 前の頂点
      * @param curr 現在の頂点
      * @param next 次の頂点
      * @return 凸ポリゴンならtrue、そうでなければfalse
      */
    bool isConvex(const glm::vec2& prev, const glm::vec2& curr, const glm::vec2& next);

    /**
      * @brief 点が三角形の内部にあるかどうかを判定する
      * @param p 判定する点
      * @param a 三角形の頂点A
      * @param b 三角形の頂点B
      * @param c 三角形の頂点C
      * @return 内部にあればtrue、そうでなければfalse
      */
    bool isPointInTriangle(const glm::vec2& p,
                           const glm::vec2& a,
                           const glm::vec2& b,
                           const glm::vec2& c);

    /**
     * @brief 三角形分割を行う
     * @param vertices 分割する頂点のリスト
     * @return 分割された三角形の頂点のリスト
     */
    std::vector<glm::vec2> triangulate(const std::vector<glm::vec2>& vertices);

protected:
    bool m_dirtyLine;
    bool m_dirtyTriangle;
    int m_bufferCapacityLine;
    int m_bufferCountLine;
    int m_bufferCapacityTriangle;
    int m_bufferCountTriangle;
    std::vector<Vertex3fC4f> m_lineBuffers;
    std::vector<Vertex3fC4f> m_triangleBuffers;
    CustomCommand m_customCommandLine;
    CustomCommand m_customCommandTriangle;
};

NS_OCF_END