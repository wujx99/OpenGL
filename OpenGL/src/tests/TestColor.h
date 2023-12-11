#pragma once
#include "Test.h"
namespace test
{
	class TestColor : public Test
	{
	public:
		TestColor();
		~TestColor();
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		float m_Color[4];
	};
}