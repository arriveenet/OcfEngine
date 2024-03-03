#pragma once
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Program.h"
#include "ProgramState.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "base/Types.h"

OCF_BEGIN

class RenderCommand {
public:
	void init(const glm::mat4& modelViewMatrix);

	const glm::mat4& getModelView() const { return m_modelVew; }
	ProgramState& getProgramState() { return m_programState; }

protected:
	RenderCommand();
	virtual ~RenderCommand();

	glm::mat4 m_modelVew;
	ProgramState m_programState;
};


OCF_END
