#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>



Shader::Shader(const std::string& filePath)
	:m_filePath(filePath), m_RendererID(0)
{
	ShaderSource ss = ParseShader(m_filePath);
	m_RendererID = CreatShader(ss.vertexSource, ss.fragmentSource);
}

Shader::~Shader()
{
	GLCallV(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCallV(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCallV(glUseProgram(0));
}

ShaderSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream fs(filePath);
	std::string line;
	std::stringstream ss[2];

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1,
	};
	ShaderType type = ShaderType::NONE;
	while (std::getline(fs, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;

		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = GLCall(glCreateShader(type));
	const char* src = source.c_str();
	GLCallV(glShaderSource(id, 1, &src, nullptr));
	GLCallV(glCompileShader(id));

	int result;
	GLCallV(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCallV(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCallV(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		GLCallV(glDeleteShader(id));
		return 0;
	}
	return id;
}

int Shader::GetLocation(const std::string& name)
{
	if (m_LocationCache.find(name) != m_LocationCache.end())
		return m_LocationCache[name];

	int location = GLCall(glGetUniformLocation(m_RendererID, name.c_str()));
	
	if (location == -1)
		std::cout << "fail to find uniform :" << name << std::endl;
	m_LocationCache.insert({ name, location });
	return location;
}

unsigned int Shader::CreatShader(const std::string& vertexShader, const std::string& fragShader)
{

	unsigned int program = GLCall(glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

	GLCallV(glAttachShader(program, vs));
	GLCallV(glAttachShader(program, fs));
	GLCallV(glLinkProgram(program));

	GLCallV(glValidateProgram(program));

	GLCallV(glDeleteShader(vs));
	GLCallV(glDeleteShader(fs));

	return program;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = GetLocation(name);
	GLCallV(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	int location = GetLocation(name);
	GLCallV(glUniform1i(location, value));
}
void Shader::SetUniformMat4f(const std::string& name,const glm::mat4& value)
{
	int location = GetLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]); // cols major ,so set transpose is "GL_FALSE"
}