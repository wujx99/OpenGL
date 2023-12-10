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
#include "VertexArray.h"
#include "BufferLayout.h"
#include "ShaderDataType.h"
#include "Texture.h"


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

	
	{
		float positions[] = {
			-0.5, -0.5, 0.0, 0.0,
			 0.5, -0.5, 1.0, 0.0,
			 0.5,  0.5, 1.0, 1.0,
			-0.5,  0.5, 0.0, 1.0,
		};


		unsigned int indexs[] = {
			0, 1, 2,
			2, 3, 0
		};

		/*
		what is blending£¿ how  do we render something that partially or fully transparent!

		if we want to draw two square,one is read,one is blue,so what is the color of the intersection of this two square? 
		the square color of top ,or blend to purple?

		we have three ways to control blending
		1. glEnable(GL_BLEND); glDisable(GL_BLEND);
		2. glBlendFunc(src=GL_ONE, dest=GL_ZERO),src is factor of color output from fragment shader
		and dest is factor to buffer our fragment shader is drawing to .GL_ONE and GL_ZERO is default.
		3. glBlendEquation(mode = GL_FUNC_ADD); mode = how we combine the src and dest color!
		*/
		GLCallV(glEnable(GL_BLEND));
		/*
		the follow function can be explain as :
		if src alpha = 0.1 -----> GL_SRC_ALPHA = 0.1 ; 
		dest = 1 - 0.1 = 0.9----> GL_ONE_MINUS_SRC_ALPHA = 0.9;
		so the final color is
			R = r_src * 0.1 + r_dest * (1- 0.1) -----> r_dest;
			G = g_src * 0.1 + g_dest * (1- 0.1) -----> g_dest;
			B = b_src * 0.1 + b_dest * (1- 0.1) -----> b_dest;
			A = a_src * 0.1 + a_dest * (1- 0.1) -----> a_dest;
			*/
		GLCallV(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); 

		VertexArray va;
		va.Bind();

		VertexBuffer vb(positions, 16 * sizeof(float));
		vb.Bind();

		BufferLayout layout = {
			{"vertex", ShaderDataType::Float2},
			{"texcoord", ShaderDataType::Float2},
		};
		va.SetLayout(layout);

		IndexBuffer ib(indexs, 6);
		ib.Bind();

		Shader shader("asset/shaders/basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.1, 0.5, 0.8, 1.0);
		
		Texture texture("asset/textures/test.png");
		texture.Bind(7);  //bind to slot 7, set uniform to 7!
		shader.SetUniform1i("u_Texture", 7);

		va.UnBind();
		shader.UnBind();
		ib.UnBind();
		vb.UnBind();

		Renderer renderer;
		float r = 0.0f;
		float increment = 0.05f;

		while (!glfwWindowShouldClose(window))
		{

			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.5, 0.8, 1.0);

			renderer.Draw(va, ib, shader);


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