#include "Program.h"
#include <glm/gtc/type_ptr.hpp>
#include "base/FileUtils.h"
#include "renderer/OpenGLUtility.h"

namespace {
void setUniform(GLuint location, unsigned int size, GLenum uniformType, void* data)
{
    GLsizei count = size;
    switch (uniformType) {
    case GL_FLOAT_VEC3:
        glUniform3fv(location, count, static_cast<GLfloat*>(data));
        break;
    case GL_FLOAT_MAT4:
        glUniformMatrix4fv(location, count, GL_FALSE, static_cast<GLfloat*>(data));
        break;
    default:
        OCFASSERT(false, "Invalidate uniform data type\n");
        break;
    }
}
}

namespace ocf {

Program::Program(const std::string& vertexShader, const std::string& fragmentShader)
    : m_uniformBufferSize(0)
{
    // 頂点シェーダーと、フラグメントシェーダーを読み込む
    const std::string shaderPath("shaders/");
    auto fileUtils    = FileUtils::getInstance();
    auto vertexFile   = fileUtils->fullPathForFilename(shaderPath + vertexShader);
    auto fragmentFile = fileUtils->fullPathForFilename(shaderPath + fragmentShader);

    std::string assetsPath = FileUtils::getInstance()->getAssetsPath();
    m_vertexShader.load(ShaderStage::Vertex, vertexFile);
    m_fragmentShader.load(ShaderStage::Fragment, fragmentFile);

    // シェーダーをコンパイル
    compileProgram();
    computeUniformInfos();
}

Program::~Program()
{
    m_vertexShader.unload();
    m_fragmentShader.unload();

    if (m_program != 0)
        glDeleteProgram(m_program);
}

int Program::getAttributeLocation(const std::string& name) const
{
    return glGetAttribLocation(m_program, name.c_str());
}

int Program::getUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_program, name.c_str());
}

void Program::use() const
{
    glUseProgram(m_program);
}

void Program::bindUniformBuffers(const char* data)
{
    for (auto&& iter : m_activeUniformInfos) {
        auto& uniformInfo = iter.second;
        if (uniformInfo.size <= 0)
            continue;

        const int elementCount = uniformInfo.count;
        ::setUniform(uniformInfo.location, elementCount, uniformInfo.type, (void*)(data + uniformInfo.bufferOffset));
    }
}

UniformInfo Program::getUniformInfo(std::string_view name)
{
    return m_activeUniformInfos[name.data()];
}

size_t Program::getUniformBufferSize() const
{
    return m_uniformBufferSize;
}

void Program::compileProgram()
{
    // シェーダーの読み込みが成功しているかチェック
    GLuint vertex = m_vertexShader.getShader();
    GLuint fragment = m_fragmentShader.getShader();
    if (vertex == 0 || fragment == 0) {
        return;
    }

    m_program = glCreateProgram();

    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);

    glLinkProgram(m_program);

    if (!isValidProgram()) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void Program::computeUniformInfos()
{
    GLint uniformCount;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);

    for (int i = 0; i < uniformCount; i++) {
        UniformInfo uniform;
        char buffer[512] = { 0 };
        GLint nameLength;
        glGetActiveUniform(m_program, i, 511, &nameLength, &uniform.count, &uniform.type, buffer);

        uniform.size = OpenGLUtility::getGLDataTypeSize(uniform.type);
        uniform.bufferOffset = static_cast<unsigned int>(m_uniformBufferSize);
        m_uniformBufferSize += uniform.size;

        std::string uniformName(buffer);

        uniform.location = glGetUniformLocation(m_program, uniformName.c_str());

        m_activeUniformInfos[uniformName] = uniform;
    }
}

bool Program::isValidProgram() const
{
    GLint status;

    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        char buffer[512] = { 0 };
        glGetProgramInfoLog(m_program, 511, nullptr, buffer);
        printf("OpenGL Link status: %s\n", buffer);
        return false;
    }

    return true;
}

} // namespace ocf
