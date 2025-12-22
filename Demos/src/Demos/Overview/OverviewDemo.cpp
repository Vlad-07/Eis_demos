#include "OverviewDemo.h"

#include "Demos/Menu.h"


OverviewDemo::OverviewDemo(const std::string& name): Layer(name) {}


void OverviewDemo::Attach()
{
	ice = Eis::Texture2D::Create("assets/textures/ice.png");
	mouce = Eis::Texture2D::Create("assets/textures/mouce.png");

	Eis::Renderer2D::SetClearColor(glm::vec3(0.1f));
}

void OverviewDemo::Detach()
{
	ice.reset();
	mouce.reset();
}


void OverviewDemo::Update()
{
	m_CameraController.Update();
}

void OverviewDemo::Render()
{
	{
		EIS_PROFILE_SCOPE("Renderer Prep");
		Eis::Renderer2D::ResetStats();
		Eis::Renderer2D::Clear();
	}

	{
		EIS_PROFILE_SCOPE("Renderer Draw");

		Eis::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Eis::Renderer2D::DrawQuad(glm::vec2(-1.0f), glm::vec2(1.0f), glm::vec4(1, 0, 0, 1));
		Eis::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f), ice);

		static float rot = 0.0f; rot += 28.8f * (float)Eis::Time::GetDeltaTime();
		if (rot >= 360.f) rot -= 360.0f;
		Eis::Renderer2D::DrawRotatedQuad(glm::vec2(2.0f, 1.0f), glm::vec2(1.0f, 0.3f), rot, glm::vec4(0.8f, 0.5f, 0.2f, 1.0f));
		Eis::Renderer2D::DrawRotatedQuad(glm::vec2(2.0f, 0.0f), glm::vec2(1.0f), rot, mouce);

		static float x = 0.0f, fact = 1.0f;
		if (x > 2.0f)		fact *= -1, x = 2.0f;
		else if (x < -2.0f) fact *= -1, x = -2.0f;
		Eis::Renderer2D::DrawCircle(glm::vec2(x += (float)Eis::Time::GetDeltaTime() * fact, 1.0f), glm::vec2(1.0f), glm::vec4(1, 1, 0, 1), m_CircleThickness, m_CircleFade);

		m_LineAngle += 100.0f * (float)Eis::Time::GetDeltaTime(); if (m_LineAngle > 360.0f) m_LineAngle -= 360.0f;
		Eis::Renderer2D::DrawLine(glm::vec2(1.0f, 0.0f), m_LineAngle, 0.5f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		Eis::Renderer2D::EndScene();
	}
}

void OverviewDemo::ImGuiRender()
{
	ImGui::Begin(m_Name.c_str());

	ImGui::Text("Line Angle: %.1f", m_LineAngle);
	ImGui::SetNextItemWidth(50.0f);
	ImGui::SliderFloat("Circle Thickness", &m_CircleThickness, 0.0f, 1.0f);
	ImGui::SetNextItemWidth(50.0f);
	ImGui::SliderFloat("Circle Fade", &m_CircleFade, 0.0f, 1.0f);

	ImGui::End();

	ImGuiMenu(Eis);
}

void OverviewDemo::OnEvent(Eis::Event& e)
{
	m_CameraController.OnEvent(e);
}


Eis::Layer::Factory OverviewDemo::GetFactory()
{
	return [](const std::string& name) -> Eis::Scope<Layer> { return Eis::CreateScope<OverviewDemo>(name); };
}