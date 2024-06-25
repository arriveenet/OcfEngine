#include "ProgramState.h"
#include "VertexArray.h"

NS_OCF_BEGIN

ProgramState::ProgramState()
	: m_pProgram(nullptr)
	, m_pTexture(nullptr)
{
}

ProgramState::~ProgramState()
{
}

void ProgramState::setProgram(Program* program)
{
	m_pProgram = program;
}

void ProgramState::setTexture(Texture2D* pTexture)
{
	m_pTexture = pTexture;
}

void ProgramState::setUniform(int location, const glm::mat4& matrix)
{
	assert(location != -1);

	m_uniforms.insert_or_assign(location, matrix);
}

void ProgramState::setUniform(const std::string& name, const glm::mat4& matrix)
{
	if (m_pProgram != nullptr) {
		const int location = m_pProgram->getUniformLocation(name);
		setUniform(location, matrix);
	}
}

void ProgramState::bindUniforms()
{
	if (m_pProgram != nullptr) {
		for (const auto& uniform : m_uniforms) {
			const int location = uniform.first;
			const glm::mat4 matrix = uniform.second;

			m_pProgram->setUniform(location, matrix);
		}
	}
}

NS_OCF_END
