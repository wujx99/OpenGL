#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;
};


static ShaderSource ParseShader(const std::string& filePath)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static unsigned int CreatShader(const std::string& vertexShader, const std::string& fragShader)
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

int main(void)
{
	GLFWwindow* window;


	if (!glfwInit())
		return -1;

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	/*
	use scope to make sure the stack allocate object vb, ib if free before glfwTerminate()
	*/
	{
		float positions[8] = {
			-0.5, -0.5,
			 0.5, -0.5,
			 0.5,  0.5,
			-0.5,  0.5
		};


		unsigned int indexs[] = {
			0, 1, 2,
			2, 3, 0
		};


		unsigned int vao;
		GLCallV(glGenVertexArrays(1, &vao));
		GLCallV(glBindVertexArray(vao));

		VertexBuffer vb(positions, 8 * sizeof(float));
		vb.Bind();


		GLCallV(glEnableVertexAttribArray(0));
		GLCallV(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));

		IndexBuffer ib(indexs, 6);
		ib.Bind();


		ShaderSource ss = ParseShader("asset/shaders/basic.shader");


		unsigned int shader = CreatShader(ss.vertexSource, ss.fragmentSource);
		GLCallV(glUseProgram(shader));

		int location = GLCall(glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);
		GLCallV(glUniform4f(location, 0.1, 0.5, 0.8, 1.0));


		GLCallV(glBindVertexArray(0));
		GLCallV(glUseProgram(0));
		ib.UnBind();
		vb.UnBind();

		float r = 0.0f;
		float increment = 0.05f;

		while (!glfwWindowShouldClose(window))
		{

			GLCallV(glClear(GL_COLOR_BUFFER_BIT));

			GLCallV(glUseProgram(shader));
			GLCallV(glUniform4f(location, r, 0.5, 0.8, 1.0));

			ib.Bind();
			GLCallV(glBindVertexArray(vao));


			GLCallV(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1 || r < 0)
				increment = -increment;
			r += increment;
			glfwPollEvents();

			glfwSwapBuffers(window);
		}

	}
	glfwTerminate();

	return 0;
}