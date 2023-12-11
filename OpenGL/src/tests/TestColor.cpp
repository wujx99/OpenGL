#include "TestColor.h"
#include "imgui.h"
#include "Renderer.h"

namespace test
{
	TestColor::TestColor()
		:m_Color{.1f, .3f, .5f, 1.f}
	{

	}
	TestColor::~TestColor()
	{

	}

	void TestColor::OnUpdate(float deltaTime)
	{
		
	}

	void TestColor::OnRender()
	{
		GLCallV(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		GLCallV(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("ClearColor", m_Color);
	}

}