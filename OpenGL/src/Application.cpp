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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

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
			100, 100, 0.0, 0.0,
			500, 100, 1.0, 0.0,
			500, 500, 1.0, 1.0,
			100, 500, 0.0, 1.0,
		};


		unsigned int indexs[] = {
			0, 1, 2,
			2, 3, 0
		};

		
		GLCallV(glEnable(GL_BLEND));
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
		shader.SetUniform4f("u_Color", 0.1f, 0.5f, 0.8f, 1.0f);

		glm::vec3 translate(200.f, 200.f, 0.f); // used by imgui to set model matrix!
		glm::mat4 proj = glm::ortho(0.0f, 900.f, 0.0f, 600.f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.f, 0.f, 0.f)); 
		

		Texture texture("asset/textures/test.png");
		texture.Bind(7);  //bind to slot 7, set uniform to 7!
		shader.SetUniform1i("u_Texture", 7);

		va.UnBind();
		shader.UnBind();
		ib.UnBind();
		vb.UnBind();

		Renderer renderer;

		// imgui init and creat context
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();

		float r = 0.0f;
		float increment = 0.05f;

		while (!glfwWindowShouldClose(window))
		{

			renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			ImGui::Begin("Imgui debug!");                          			
			ImGui::SliderFloat3("float", &translate.x, 0.0f, 900.f);    // set the translate of model matrix!
			ImGui::End();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translate);
			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);


			if (r > 1 || r < 0)
				increment = -increment;
			r += increment;
			glfwPollEvents();

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}

	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}