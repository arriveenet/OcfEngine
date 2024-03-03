#pragma once
#include "Program.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

OCF_BEGIN

class ProgramState {
public:
	ProgramState();
	virtual ~ProgramState();

	void init(Program* program);

	Program* getProgram() const { return m_pProgram; }

	void createVertexBuffer(void* pData, size_t size);
	void createIndexBuffer(void* pData, size_t size);

	VertexBuffer* getVertexBuffer() const { return m_pVertexBuffer; }
	VertexBuffer* getIndexBuffer() const { return m_pIndexBuffer; }
	VertexArray* getVertexArray() const { return m_pVertexArray; }

private:
	Program* m_pProgram;
	VertexBuffer* m_pVertexBuffer;
	VertexBuffer* m_pIndexBuffer;
	VertexArray* m_pVertexArray;
};

OCF_END
