#include "FractalDemo.h"


FractalDemo::FractalDemo(const std::string& name) : Demo(name), m_FractalRenderer(m_CamController)
{
	m_CamController.SetMinZoom(0);
}


void FractalDemo::OnAttach()
{
	Eis::Renderer2D::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	m_FractalRenderer.SetCanvasSize(Eis::Application::Get().GetWindow().GetWidth(), Eis::Application::Get().GetWindow().GetHeight());
}


void FractalDemo::Update(Eis::TimeStep ts)
{
	// Compute
	m_CamController.OnUpdate(ts);

	m_FractalRenderer.Compute(m_Constant, m_MaxIt);

	// Rendering
	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());

	m_FractalRenderer.Render();

	Eis::Renderer2D::EndScene();
}

void FractalDemo::ImGuiRender()
{
	ImGui::Begin("Julia Set");

	ImGui::DragFloat2("Constant", (float*)&m_Constant, 0.0005f, -2.0f, 2.0f);
	ImGui::SliderInt("Detail", (int*)&m_MaxIt, 50, 300);

	uint8_t toLoad = 0;
	if (ImGui::Button("1")) toLoad = 1;
	ImGui::SameLine();
	if (ImGui::Button("2")) toLoad = 2;
	ImGui::SameLine();
	if (ImGui::Button("3")) toLoad = 3;
	ImGui::SameLine();
	if (ImGui::Button("4")) toLoad = 4;
	ImGui::SameLine();
	if (ImGui::Button("5")) toLoad = 5;
	ImGui::SameLine();
	if (ImGui::Button("6")) toLoad = 6;
	ImGui::SameLine();
	if (ImGui::Button("7")) toLoad = 7;
	ImGui::SameLine();
	if (ImGui::Button("8")) toLoad = 8;

	if (toLoad > 0 && toLoad < s_FractalLib.size())
	{
		m_Constant = s_FractalLib[toLoad - 1].first;
		m_MaxIt = s_FractalLib[toLoad - 1].second;
		toLoad = 0;
	}

	ImGui::End();
}

void FractalDemo::OnEvent(Eis::Event& e)
{
	m_CamController.OnEvent(e);
	m_FractalRenderer.OnEvent(e);
}