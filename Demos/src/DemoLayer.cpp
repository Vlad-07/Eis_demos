#include "DemoLayer.h"


DemoLayer::DemoLayer() : Layer("Demo"), m_DemoManager()
{
	Eis::Application::Get().GetWindow().SetTitle("Eis Demos");

	m_DemoManager.LoadDemo<OverviewDemo>("Overview");
	m_DemoManager.LoadDemo<PhysicsDemo>("Physics Demo");
	m_DemoManager.LoadDemo<SandDemo>("Sand Demo");
#ifdef EIS_NETWORKING_ENABLE
	m_DemoManager.LoadDemo<ChatDemo>("Chat Demo");
#endif
}

void DemoLayer::Attach()
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().Attach();
}

void DemoLayer::Detach()
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().Detach();
}


void DemoLayer::Update()
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().Update();
}

void DemoLayer::FixedUpdate()
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().FixedUpdate();
}

void DemoLayer::Render()
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().Render();
}

void DemoLayer::ImGuiRender()
{
	EIS_PROFILE_FUNCTION();

	if (ImGui::Begin("Demo Menu")) {
		for (int i = 0; i < m_DemoManager.GetDemos().size(); i++)
		{
			if (ImGui::Button(m_DemoManager.GetDemos()[i]->GetName().c_str()) && i != m_DemoManager.GetCurrentDemoId())
			{
				m_DemoManager.GetCurrentDemo().Detach();
				m_DemoManager.SetCurrentDemo(i);
				Eis::Renderer2D::ResetStats();
				EIS_INFO("Selected {0} demo", m_DemoManager.GetCurrentDemo().GetName());
				m_DemoManager.GetCurrentDemo().Attach();
			}
		}
#ifndef EIS_PLATFORM_WEB
		if (ImGui::Button("Quit")) Eis::Application::ShouldClose();
#endif
	} ImGui::End();

	ImGui::Begin("Performance");
	// TODO: maybe a proper performance system?
	static uint32_t frames = 0; frames++;
	static float elapsed = 0; elapsed += Eis::Time::GetDeltaTime();
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

	// MVPs must be updated for all demo's cameras
	// other events *might* need to be propagated to all demos
	if (e.IsType(Eis::EventType::WindowResize))
	{
		for (Eis::Scope<Demo>& demo : m_DemoManager.GetDemos())
			demo->OnEvent(e);
		return;
	}

	m_DemoManager.GetCurrentDemo().OnEvent(e);
}