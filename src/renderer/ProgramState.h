#pragma once
#include "Program.h"

OCF_BEGIN

class Texture2D;
class VertexArray;

class ProgramState {
public:
	ProgramState();
	virtual ~ProgramState();

	void setProgram(Program* program);
	void setTexture(Texture2D* pTexture);

	Program* getProgram() const { return m_pProgram; }

private:
	Program* m_pProgram;
	Texture2D* m_pTexture;
};

OCF_END
