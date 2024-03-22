#include "ProgramState.h"
#include "VertexArray.h"

OCF_BEGIN

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

OCF_END
