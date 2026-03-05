#pragma once
#include <unordered_map>
#include <string>
#include "Program.h"

namespace ocf {

class Texture2D;
class VertexArray;

class ProgramState {
public:
    ProgramState();
    virtual ~ProgramState();

    void setProgram(Program* program);
    void setTexture(Texture2D* pTexture);

    void setUniform(std::string_view name, const void* pData, size_t size);
    char* getUniformBuffers() const;

    Program* getProgram() const { return m_pProgram; }

private:
    Program* m_pProgram;
    Texture2D* m_pTexture;
    std::unordered_map<int, glm::mat4> m_uniforms;
    char* m_pUniformBuffers;
};

} // namespace ocf
