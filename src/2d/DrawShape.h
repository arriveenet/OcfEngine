#pragma once
#include <vector>
#include "Node.h"
#include "renderer/VertexArray.h"
#include "renderer/CustomCommand.h"

NS_OCF_BEGIN

class DrawShape : public Node {
public:
    static DrawShape* create();

    DrawShape();
    virtual ~DrawShape();

    bool init() override;
    void clear();

    void ensureCapacityGLLine(int count);

    void drawLine(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);
    void drawLine(const glm::vec3& origin, const glm::vec3& destanation, const glm::vec4& color);
    void drawRect(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);

    void update(float deltaTime) override;
    void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
    void updateVertexBuffer();
    void updateUniforms(const glm::mat4& transform, CustomCommand& cmd);

protected:
    bool m_dirtyLine;
    int m_bufferCapacityLine;
    int m_bufferCountLine;
    std::vector<Vertex3fC4f> m_lineBuffers;
    CustomCommand m_customCommandLine;
};

NS_OCF_END