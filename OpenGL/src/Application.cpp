#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
the readable way to return muti-return value
*/
struct ShaderSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

/*
read the all shaders from one file to string
*/
static ShaderSource ParseShader(const std::string& filePath)
{
    std::ifstream fs(filePath);
    std::string line;

    std::stringstream ss[2];
    // good practice!
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
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreatShader(const std::string& vertexShader, const std::string& fragShader)
{
    // inconsist with other api£¡
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;


    if (!glfwInit())
        return -1;


    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);


    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    GLuint a;
    glGenBuffers(1, &a);

    std::cout << glGetString(GL_VERSION) << std::endl;
    float positions[6] = {
        0.5,  0.5,
        0.0,  1.0,
        0.2, -1.0,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions,GL_STATIC_DRAW);

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    /*
    the filepath is relative! in visual studio ,you can find the 
    Working Directory is $(PrijectDir) in debuging set in properites
    */
    ShaderSource ss = ParseShader("asset/shaders/basic.shader");
    std::cout << "VERTEX" << std::endl;
    std::cout << ss.vertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << ss.fragmentSource << std::endl;
    
    
    unsigned int shader = CreatShader(ss.vertexSource, ss.fragmentSource);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
     
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3); 
        
        glfwPollEvents();

        glfwSwapBuffers(window);
    }
   
    
    glfwTerminate();
    
    return 0;
}