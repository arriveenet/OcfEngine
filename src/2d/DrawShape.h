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

	void drawLine(const glm::vec2& origin, const glm::vec2& destanation, const glm::vec4& color);

	void update(float deltaTime) override;
	void draw(Renderer* renderer) override;

protected:
	void updateVertexBuffer();

protected:
	bool m_dirtyLine;
	int m_bufferCapacity;
	std::vector<Vertex2fC4> m_lineBuffers;
	VertexArray m_vertexArray;
	CustomCommand m_customCommand;
};

OCF_END