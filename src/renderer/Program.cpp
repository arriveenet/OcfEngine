#include "Program.h"
#include <glm/gtc/type_ptr.hpp>
#include "base/FileUtils.h"

NS_OCF_BEGIN

Program::Program(const std::string& vertexShader, const std::string& fragmentShader)
{
	// 頂点シェーダーと、フラグメントシェーダーを読み込む
	const std::string shaderPath("shaders\\");
	auto fileUtils    = FileUtils::getInstance();
	auto vertexFile   = fileUtils->fullPathForFilename(shaderPath + vertexShader);
	auto fragmentFile = fileUtils->fullPathForFilename(shaderPath + fragmentShader);

	std::string assetsPath = FileUtils::getInstance()->getAssetsPath();
	m_vertexShader.load(ShaderStage::Vertex, vertexFile);
	m_fragmentShader.load(ShaderStage::Fragment, fragmentFile);

	// シェーダーをコンパイル
	compileProgram();
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

void Program::use()
{
	glUseProgram(m_program);
}

void Program::setUniform(int location, const glm::mat4& matrix) const
{
	assert(location != -1);

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::setUniform(const std::string& name, const glm::mat4& matrix) const
{
	const int location = getUniformLocation(name);
	assert(location != -1);

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
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

bool Program::isValidProgram()
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

NS_OCF_END
