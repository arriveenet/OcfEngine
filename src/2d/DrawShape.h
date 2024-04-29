#pragma once
#include <vector>
#include "Entity.h"
#include "renderer/VertexArray.h"
#include "renderer/CustomCommand.h"

OCF_BEGIN

class DrawShape : public Entity {
public:
	static DrawShape* create();

	DrawShape();
	virtual ~DrawShape();

	bool init() override;
	void clear();

	void ensureCapacityGLLine(int count);

	void drawLine(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);
	void drawRect(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);

	void update(float deltaTime) override;
	void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
	void updateVertexBuffer();

protected:
	bool m_dirtyLine;
	int m_bufferCapacityLine;
	int m_bufferCountLine;
	std::vector<Vertex2fC4> m_lineBuffers;
	CustomCommand m_customCommandLine;
};

OCF_END