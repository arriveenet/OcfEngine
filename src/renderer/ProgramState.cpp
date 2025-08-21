#include "ProgramState.h"
#include "VertexArray.h"

namespace ocf {

ProgramState::ProgramState()
    : m_pProgram(nullptr)
    , m_pTexture(nullptr)
    , m_pUniformBuffers(nullptr)
{
}

ProgramState::~ProgramState()
{
    OCF_SAFE_FREE(m_pUniformBuffers);
}

void ProgramState::setProgram(Program* program)
{
    m_pProgram = program;

    if (m_pUniformBuffers) {
        free(m_pUniformBuffers);
        m_pUniformBuffers = nullptr;
    }

    m_pUniformBuffers = static_cast<char*>(malloc(m_pProgram->getUniformBufferSize()));
}

void ProgramState::setTexture(Texture2D* pTexture)
{
    m_pTexture = pTexture;
}

void ProgramState::setUniform(std::string_view name, const void* pData, size_t size)
{
    if (!m_pProgram)
        return;
    UniformInfo uniform = m_pProgram->getUniformInfo(name);
    if (uniform.location < 0)
        return;

    memcpy(m_pUniformBuffers + uniform.bufferOffset, pData, size);
}

char* ProgramState::getUniformBuffers() const
{
    return m_pUniformBuffers;
}

} // namespace ocf
