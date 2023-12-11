#include "TestTexture2D.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
namespace test
{
	TestTexture2D::TestTexture2D()
		:m_VertexArray(std::make_unique<VertexArray>()),
		m_Shader(std::make_unique<Shader>("asset/shaders/basic.shader")),
		m_Texture(std::make_unique<Texture>("asset/textures/test.png")),
		m_TranslateA(200.f, 200.f, 0.f),
		m_TranslateB(400.f, 200.f, 0.f),
		m_Proj(glm::ortho(0.0f, 900.f, 0.0f, 600.f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f)))
	{
		/*
			center to 0 , make it easy to calulate the MVP matrix
			*/
		float positions[] = {
			-50, -50, 0.0, 0.0,
			 50, -50, 1.0, 0.0,
			 50,  50, 1.0, 1.0,
			-50,  50, 0.0, 1.0,
		};


		unsigned int indexs[] = {
			0, 1, 2,
			2, 3, 0
		};


		GLCallV(glEnable(GL_BLEND));
		GLCallV(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 16 * sizeof(float));
		m_VertexBuffer->Bind();

		BufferLayout layout = {
			{"vertex", ShaderDataType::Float2},
			{"texcoord", ShaderDataType::Float2},
		};

		m_VertexArray->Bind();
		m_VertexArray->SetLayout(layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indexs, 6); 
		m_IndexBuffer->Bind();

		
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.1f, 0.5f, 0.8f, 1.0f);
		
		m_Texture->Bind(7);  //bind to slot 7, set uniform to 7!
		m_Shader->SetUniform1i("u_Texture", 7);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		Renderer renderer;
		{
			// first object
			m_Model = glm::translate(glm::mat4(1.0f), m_TranslateA);
			glm::mat4 mvp = m_Proj * m_View * m_Model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
		{
			// second object
			m_Model = glm::translate(glm::mat4(1.0f), m_TranslateB);
			glm::mat4 mvp = m_Proj * m_View * m_Model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("TranslateA", &m_TranslateA.x, 0.0f, 900.f);  
		ImGui::SliderFloat3("TranslateB", &m_TranslateB.x, 0.0f, 900.f);  
	}

}
