#pragma once
#include <string>
#include "OpenGLInclude.h"
#include "base/Macros.h"

OCF_BEGIN

enum class ShaderStage : uint32_t{
	Vertex,
	Fragment
};

class Shader {
public:
	Shader();
	~Shader();

	bool load(ShaderStage stage, const std::string& shaderSource);
	void unload();

	GLuint getShader() const { return m_shader; }

private:
	bool compileShader(ShaderStage stage,const std::string& source);
	bool isCompiled(GLuint shader);

private:
	GLuint m_shader = 0;
};

OCF_END
