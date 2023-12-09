#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef DEBUG
	#define GLCallV( x ) \
		 GLClearErrors(); \
		 x; \
		 if ( !GLCheckErrors( #x, __FILE__, __LINE__) ) __debugbreak();
	#define GLCall( x ) [&]() { \
		 GLClearErrors(); \
		 auto retVal = x; \
		 if ( !GLCheckErrors( #x, __FILE__, __LINE__) ) __debugbreak(); \
		 return retVal; \
	   }()
#else
	#define GLCallV( x ) x;
	#define GLCall( x ) x;
#endif
static void GLClearErrors()
{
	while (glGetError()!=GL_NO_ERROR)
	{

	}
}

static bool GLCheckErrors(const char*func, const char*file, unsigned int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "get error :" << error <<
			" " << func << " :" << file << ":"<< line << std::endl;
		
		return false;
	}
	return true;
}

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

	/*
	GLFW_OPENGL_CORE_PROFILE donot think "0" is a VAO index. if we donot bind a VAO, the glEnableVertexAttribArray(0) will bind to "0" VAO by default.
	so wo can:
	1. Bind a VAO or
	2. set GLFW_OPENGL_CORE_PROFILE to GLFW_OPENGL_COMPAT_PROFILE
	*/
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
	GLuint a;
	GLCallV(glGenBuffers(1, &a));

	std::cout << glGetString(GL_VERSION) << std::endl;
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

	// the method 2 to fix the error of CORE_PROFILE
	unsigned int vao;
	GLCallV(glGenVertexArrays(1, &vao));
	GLCallV(glBindVertexArray(vao));
	

	unsigned int buffer;
	GLCallV(glGenBuffers(1, &buffer));
	GLCallV(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCallV(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));


	GLCallV(glEnableVertexAttribArray(0));
	/*
	here the buffer and vao link and bound!
	*/
	GLCallV(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));

	unsigned int ibo; 
	GLCallV(glGenBuffers(1, &ibo));
	GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCallV(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexs, GL_STATIC_DRAW));


	ShaderSource ss = ParseShader("asset/shaders/basic.shader");


	unsigned int shader = CreatShader(ss.vertexSource, ss.fragmentSource);
	GLCallV(glUseProgram(shader));

	int location = GLCall(glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCallV(glUniform4f(location, 0.1, 0.5, 0.8, 1.0));

	/*
	just show how to rebind, we just need to bind VAO and ibo;
	*/
	// unbind
	GLCallV(glBindVertexArray(0));
	GLCallV(glUseProgram(0));
	GLCallV(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	float increment = 0.05f;
	
	while (!glfwWindowShouldClose(window))
	{

		GLCallV(glClear(GL_COLOR_BUFFER_BIT));

		GLCallV(glUseProgram(shader));
		GLCallV(glUniform4f(location, r, 0.5, 0.8, 1.0));
	
		GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCallV(glBindVertexArray(vao)); 

		
		GLCallV(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		
		if (r > 1 || r < 0)
			increment = -increment;
		r += increment;
		glfwPollEvents();

		glfwSwapBuffers(window);
	}


	glfwTerminate();

	return 0;
}