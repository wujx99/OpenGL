#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"


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

		Shader shader("asset/shaders/basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.1, 0.5, 0.8, 1.0);
	

		GLCallV(glBindVertexArray(0));
		shader.UnBind();
		ib.UnBind();
		vb.UnBind();

		float r = 0.0f;
		float increment = 0.05f;

		while (!glfwWindowShouldClose(window))
		{

			GLCallV(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.5, 0.8, 1.0);

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