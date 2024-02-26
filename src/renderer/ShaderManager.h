#pragma once
#include <unordered_map>
#include "GameObject.h"
#include "Program.h"

OCF_BEGIN

enum class ProgramType : uint32_t {
	Basic,
	Max
};

class ShaderManager : public GameObject {
public:
	static ShaderManager* getInstance();
	static void destroyInstance();

	Program* getProgram(ProgramType type);
	void removeAllPrograms();

private:
	bool init();

	static ShaderManager* s_sharedShaderManager;

	std::unordered_map<ProgramType, Program*> m_programs;
};

OCF_END
