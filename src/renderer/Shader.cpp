#include "Shader.h"
#include <fstream>
#include <sstream>
#include <cassert>

NS_OCF_BEGIN

static GLenum toGLShaderType(ShaderStage stage)
{
	switch (stage) {
	case ocf::ShaderStage::Vertex:
		return GL_VERTEX_SHADER;
		break;
	case ocf::ShaderStage::Fragment:
		return GL_FRAGMENT_SHADER;
		break;
	default:
		assert(false);
		return 0;
		break;
	}
}

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::load(ShaderStage stage, const std::string& shaderSource)
{
	return compileShader(stage, shaderSource);
}

void Shader::unload()
{
	if (m_shader != 0)
		glDeleteShader(m_shader);
}

bool Shader::compileShader(ShaderStage stage, const std::string& source)
{
	std::ifstream shaderFile(source);
	if (!shaderFile) {
		printf("Shader file not found %s\n", source.c_str());
		return false;
	}

	std::stringstream sstream;
	sstream << shaderFile.rdbuf();
	std::string contents = sstream.str();
	const char* contentChar = contents.c_str();

	m_shader = glCreateShader(toGLShaderType(stage));
	glShaderSource(m_shader, 1, &(contentChar), nullptr);
	glCompileShader(m_shader);

	if (!isCompiled(m_shader)) {
		printf("Failed to compile shader %s\n", source.c_str());
		return false;
	}

	return true;
}

bool Shader::isCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512] = { 0 };
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		printf("OpenGL Compile Failed: %s\n", buffer);
		return false;
	}

	return true;
}

NS_OCF_END
