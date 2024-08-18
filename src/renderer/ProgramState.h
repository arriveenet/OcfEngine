#pragma once
#include <unordered_map>
#include <string>
#include "Program.h"

NS_OCF_BEGIN

class Texture2D;
class VertexArray;

class ProgramState {
public:
    ProgramState();
    virtual ~ProgramState();

    void setProgram(Program* program);
    void setTexture(Texture2D* pTexture);

    void setUniform(int location, const glm::mat4& matrix);
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void bindUniforms();

    Program* getProgram() const { return m_pProgram; }

private:
    Program* m_pProgram;
    Texture2D* m_pTexture;
    std::unordered_map<int, glm::mat4> m_uniforms;
};

NS_OCF_END
