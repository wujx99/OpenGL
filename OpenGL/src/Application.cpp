#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

   
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    GLuint a;
    glGenBuffers(1, &a);

    std::cout << glGetString(GL_VERSION) << std::endl;

  
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2d(0.5, 0.5);
        glVertex2d(0.0, 1.0);
        glVertex2d(0.2, - 1.0);
        glEnd();
        

        /* Poll for and process events */
        glfwPollEvents();

        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    
    return 0;
}