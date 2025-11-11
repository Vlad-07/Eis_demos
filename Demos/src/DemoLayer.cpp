#include "DemoLayer.h"


DemoLayer::DemoLayer() : Layer("Demo"), m_DemoManager()
{
	Eis::Application::Get().GetWindow().SetTitle("Eis Demos");

	m_DemoManager.LoadDemo(new OverviewDemo("Overview"));
	m_DemoManager.LoadDemo(new PhysicsDemo("Physics Demo"));
	m_DemoManager.LoadDemo(new SandDemo("Sand Demo"));
#ifdef EIS_NETWORKING_ENABLE
	m_DemoManager.LoadDemo(new ChatDemo("Chat Demo"));
#endif
}

void DemoLayer::OnAttach()
{
	EIS_PROFILE_FUNCTION();

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
	m_LastTs = ts;
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
				Eis::Renderer2D::ResetStats();
				EIS_INFO("Selected {0} demo", m_DemoManager.GetCurrentDemo().GetName());
				m_DemoManager.GetCurrentDemo().OnAttach();
			}
		}
		if (ImGui::Button("Quit")) Eis::Application::ShouldClose();
	} ImGui::End();

	ImGui::Begin("Performance");
	// TODO: maybe a proper performance system?
	static uint32_t frames = 0; frames++;
	static float elapsed = 0; elapsed += m_LastTs;
	static float fps = 0.0f;
	if (elapsed > 0.25f)
		fps = frames / elapsed, frames = 0, elapsed = 0.0f;

	ImGui::Text("%.1f FPS (%.3f ms)", fps, 1000.0f / fps);
	ImGui::Text("Draw calls:   %i", Eis::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quad count:   %i", Eis::Renderer2D::GetStats().QuadCount);
	ImGui::Text("Circle count: %i", Eis::Renderer2D::GetStats().CircleCount);
	ImGui::Text("Line count:   %i", Eis::Renderer2D::GetStats().LineCount);
	ImGui::End();

	m_DemoManager.GetCurrentDemo().ImGuiRender();
}

void DemoLayer::OnEvent(Eis::Event& e)
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().OnEvent(e);
}