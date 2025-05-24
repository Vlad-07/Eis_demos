#include "FractalDemo.h"


FractalDemo::FractalDemo(const std::string& name) : Demo(name)
{
	m_FractalRenderer.SetCanvasSize(Eis::Application::Get().GetWindow().GetWidth(), Eis::Application::Get().GetWindow().GetHeight());
	m_CamController.SetPoseLock(true);
	m_CamController.SetZoomLock(true);
}


void FractalDemo::OnAttach()
{
	Eis::RenderCommands::Disable(0x0B71); // GL_DEPTH_TEST
	Eis::Renderer2D::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}


void FractalDemo::Update(Eis::TimeStep ts)
{
	// Compute
	m_CamController.OnUpdate(ts);

	if (m_Auto)
	{
		static float sign = 1.0f;
		m_Constant += glm::vec2(-0.001f, 0.001f) * sign;
		if (m_Constant.x < 0.0f || m_Constant.x > 1.0f)
			sign *= -1.0f;
	}
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
	ImGui::SliderInt("Detail", &m_MaxIt, 50, 300);

	if (ImGui::Button("1"))
		m_Constant = glm::vec2(-0.209f, 0.696f), m_MaxIt = 500;
	ImGui::SameLine();
	if (ImGui::Button("2"))
		m_Constant = glm::vec2(-0.4f, 0.6f), m_MaxIt = 200;
	ImGui::SameLine();
	if (ImGui::Button("3"))
		m_Constant = glm::vec2(-0.02f, 0.79f), m_MaxIt = 150;
	ImGui::SameLine();
	if (ImGui::Button("4"))
		m_Constant = glm::vec2(-0.8f, 0.156f), m_MaxIt = 500;
	ImGui::SameLine();
	if (ImGui::Button("5"))
		m_Constant = glm::vec2(0.35f, 0.358f), m_MaxIt = 500;
	ImGui::SameLine();
	if (ImGui::Button("6"))
		m_Constant = glm::vec2(-0.21f, 0.79f), m_MaxIt = 400;
	ImGui::SameLine();
	if (ImGui::Button("7"))
		m_Constant = glm::vec2(0.407f, 0.306f), m_MaxIt = 100;
	ImGui::SameLine();
	if (ImGui::Button("8"))
		m_Constant = glm::vec2(-1.166f, -0.259f), m_MaxIt = 100;

	ImGui::End();
}

void FractalDemo::OnEvent(Eis::Event& e)
{
	m_CamController.OnEvent(e);
	m_FractalRenderer.OnEvent(e);
}