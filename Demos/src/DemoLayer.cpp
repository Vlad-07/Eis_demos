#include "DemoLayer.h"


DemoLayer::DemoLayer() : Layer("Demo"), m_DemoManager()
{}

void DemoLayer::OnAttach()
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.LoadDemo(new OverviewDemo("Overview"));
	m_DemoManager.LoadDemo(new ChatDemo("Chat Demo"));
	m_DemoManager.LoadDemo(new SandDemo("Sand Demo"));
	m_DemoManager.LoadDemo(new GTDemo("Game Theory"));

	m_DemoManager.GetCurrentDemo().OnAttach();
}

void DemoLayer::OnDetach()
{
	EIS_PROFILE_FUNCTION();
	m_DemoManager.GetCurrentDemo().OnDetach();
}


void DemoLayer::OnUpdate(Eis::TimeStep ts)
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().Update(ts);
}

void DemoLayer::OnImGuiRender()
{
	EIS_PROFILE_FUNCTION();

	if (ImGui::Begin("Demo Menu")) {
	for (int i = 0; i < m_DemoManager.GetDemos().size(); i++)
	{
		if (ImGui::Button(m_DemoManager.GetDemos()[i]->GetName().c_str()) && i != m_DemoManager.GetCurrentDemoId())
		{
			m_DemoManager.GetCurrentDemo().OnDetach();
			m_DemoManager.SetCurrentDemo(i);
			EIS_INFO("Selected {0} demo", m_DemoManager.GetCurrentDemo().GetName());
			m_DemoManager.GetCurrentDemo().OnAttach();
		}
	}}
	ImGui::End();

	m_DemoManager.GetCurrentDemo().ImGuiRender();
}

void DemoLayer::OnEvent(Eis::Event& e)
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().OnEvent(e);
}