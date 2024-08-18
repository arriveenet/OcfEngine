#include "ShaderManager.h"

NS_OCF_BEGIN

ShaderManager* ShaderManager::s_sharedShaderManager = nullptr;

ShaderManager* ShaderManager::getInstance()
{
    if (s_sharedShaderManager == nullptr) {
        s_sharedShaderManager = new ShaderManager();
        s_sharedShaderManager->init();
    }

    return s_sharedShaderManager;
}

void ShaderManager::destroyInstance()
{
    delete s_sharedShaderManager;
    s_sharedShaderManager = nullptr;
}

Program* ShaderManager::getProgram(ProgramType type)
{
    const auto& iter = m_programs.find(type);
    if (iter != m_programs.end()) {
        return iter->second;
    }

    return nullptr;
}

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
    for (auto& program : m_programs) {
        delete program.second;
        program.second = nullptr;
    }

    m_programs.clear();
}

bool ShaderManager::init()
{
    Program* program = new Program("basic.vert", "basic.frag");
    m_programs.emplace(ProgramType::Basic, program);

    program = new Program("label.vert", "label.frag");
    m_programs.emplace(ProgramType::Label, program);

    program = new Program("drawShape.vert", "drawShape.frag");
    m_programs.emplace(ProgramType::DrawShape, program);

    program = new Program("positionTexture.vert", "positionTexture.frag");
    m_programs.emplace(ProgramType::PositionTexture, program);

    return true;
}

NS_OCF_END
