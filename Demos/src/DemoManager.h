#pragma once

#include <Eis.h>

#include "Demos/Demo.h"


class DemoManager
{
public:
	DemoManager() : m_Demos() {}
	~DemoManager() { for (Eis::Scope<Demo>& demo : m_Demos) demo.reset(); }

	template<typename T, typename ... Args>
	void LoadDemo(Args&& ... args)
	{
		static_assert(std::is_base_of_v<Demo, T> == true);
		m_Demos.push_back(std::move(Eis::CreateScope<T>(std::forward<Args>(args)...)));
	}

	void SetCurrentDemo(uint32_t id) { if (id >= m_Demos.size()) EIS_ERROR("Invalid demo id!"); m_CurrentDemoId = id; }

	Demo& GetCurrentDemo() { return *m_Demos[m_CurrentDemoId]; }
	std::vector<Eis::Scope<Demo>>& GetDemos() { return m_Demos; }
	uint32_t GetCurrentDemoId() const { return m_CurrentDemoId; }

private:
	std::vector<Eis::Scope<Demo>> m_Demos;
	uint32_t m_CurrentDemoId = 0;
};