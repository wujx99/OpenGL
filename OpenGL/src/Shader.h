#pragma once
#include <string>
#include <unordered_map>

struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;
};


class Shader {
private:
	unsigned int m_RendererID;
	std::string m_filePath;
	std::unordered_map<std::string, int> m_LocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	ShaderSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreatShader(const std::string& vertexShader, const std::string& fragShader);
	int GetLocation(const std::string& name);

};