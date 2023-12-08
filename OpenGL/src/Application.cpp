#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

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

  

    while (!glfwWindowShouldClose(window))
    {
     
        glClear(GL_COLOR_BUFFER_BIT);

        /*
        which data to draw?
        we should to know OpenGL is a state mechine.the glBindBuffer set the state!
        we can use glBindBuffer(GL_ARRAY_BUFFER, 0) to change the binded buffer !
         */
        glDrawArrays(GL_TRIANGLES, 0, 3); 
        
        glfwPollEvents();

        glfwSwapBuffers(window);
    }


    glfwTerminate();
    
    return 0;
}