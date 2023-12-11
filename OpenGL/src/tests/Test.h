#pragma once
#include <functional>
namespace test
{
	class Test
	{
	public:
		Test() {};
		virtual ~Test() = default;

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};

	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test* &currentTestMenu);
		~TestMenu() = default;

		template<typename T>
		void RegisterTest(const std::string& testName)
		{
			std::cout << "register test:" << testName << std::endl;
			m_Tests.push_back({ testName, []() {return new T(); } });
		}
		virtual void OnImGuiRender() override;
	private:
		Test* &m_CurrentTest; // must be reference!!!!!!!!!
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}