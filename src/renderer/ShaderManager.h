#pragma once
#include <unordered_map>
#include "base/GameObject.h"
#include "Program.h"

NS_OCF_BEGIN

enum class ProgramType : uint32_t {
    Basic,
    Label,
    DrawShape,
    PositionTexture,
    Position3D,
    Max
};

class ShaderManager : public GameObject {
public:
    static ShaderManager* getInstance();
    static void destroyInstance();

    Program* getProgram(ProgramType type);

protected:
    ShaderManager();
    virtual ~ShaderManager();

private:
    bool init();

    static ShaderManager* s_sharedShaderManager;

    std::unordered_map<ProgramType, Program*> m_programs;
};

NS_OCF_END
