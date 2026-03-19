#pragma once
#include "renderer/backend/opengl/OpenGLInclude.h"
#include <string>

namespace ocf {

enum class ShaderStage : uint32_t{
    Vertex,
    Fragment
};

class Shader {
public:
    Shader();
    ~Shader();

    bool load(ocf::ShaderStage stage, const std::string& shaderSource);
    void unload();

    GLuint getShader() const { return m_shader; }

private:
    bool compileShader(ocf::ShaderStage stage,const std::string& source);
    bool isCompiled(GLuint shader);

private:
    GLuint m_shader = 0;
};

} // namespace ocf
