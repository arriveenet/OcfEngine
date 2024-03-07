#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"
#include "base/Macros.h"

OCF_BEGIN

class Shader;

class Program {
public:
	Program(const std::string& vertexShader, const std::string& fragmentShader);
	~Program();

	GLuint getProgram() const { return m_program; }

	int getAttributeLocation(const std::string& name) const;
	int getUniformLocation(const std::string& name) const;

	void setUniform(const std::string& name, const glm::mat4& matrix) const;

private:
	void compileProgram();
	bool isValidProgram();

	GLuint m_program;
	Shader m_vertexShader;
	Shader m_fragmentShader;
};

OCF_END
