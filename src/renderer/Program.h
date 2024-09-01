#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Shader.h"
#include "base/Macros.h"

NS_OCF_BEGIN

class Shader;

struct UniformInfo {
    int count = 0;
    int location = -1;
    unsigned int type = 0;
    unsigned int size = 0;
    unsigned int bufferOffset = 0;
};

class Program {
public:
    Program(const std::string& vertexShader, const std::string& fragmentShader);
    ~Program();

    GLuint getProgram() const { return m_program; }

    int getAttributeLocation(const std::string& name) const;
    int getUniformLocation(const std::string& name) const;

    void use() const;
    void bindUniformBuffers(const char* data);
    UniformInfo getUniformInfo(std::string_view name);
    size_t getUniformBufferSize() const;

private:
    void compileProgram();
    void computeUniformInfos();
    bool isValidProgram() const;

    GLuint m_program;
    Shader m_vertexShader;
    Shader m_fragmentShader;
    std::unordered_map<std::string, UniformInfo> m_activeUniformInfos;
    size_t m_uniformBufferSize;
};

NS_OCF_END
