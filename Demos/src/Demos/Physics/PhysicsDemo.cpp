#include "PhysicsDemo.h"


PhysicsDemo::PhysicsDemo(const std::string& name) : Demo(name)
{
	m_CamController.SetPoseLock(true);
	m_CamController.SetMaxZoom(100);
	m_CamController.SetZoom(15.0f);

	Eis::PhysicsManager2D::AddBody(glm::vec2(0, -5.0f), 0.0f, glm::vec2(40.0f, 1.0f), 0, 0.3f, true);
	m_Colors.emplace_back(glm::vec4(0.2f, 0.85f, 0.2f, 1.0f));
	for (int i = 0; i < 40; i++)
	{
		if (i % 2)
			Eis::PhysicsManager2D::AddBody({ Eis::Random::Float(-15.0f, 15.0f), Eis::Random::Float(3.0f, 5.0f) }, Eis::Random::Float(0.3f, 0.7f), 1.0f, 0.5f);
		else
			Eis::PhysicsManager2D::AddBody({ Eis::Random::Float(-15.0f, 15.0f), Eis::Random::Float(7.0f, 15.0f) }, 0, Eis::Random::Vec2(0.7f, 1.3f), 1.0f, 0.5f);
		m_Colors.emplace_back(glm::vec4(Eis::Random::Vec3(), 1.0f));
	}
}


void PhysicsDemo::Attach()
{
	Eis::Renderer2D::SetClearColor(glm::vec3(0.1f));
	Eis::Renderer2D::SetLineWidth(1.0f);
	Eis::Application::Get().GetWindow().SetVSync(false);
}

void PhysicsDemo::Detach()
{
	Eis::Application::Get().GetWindow().SetVSync(true);
}


void PhysicsDemo::Update(Eis::TimeStep ts)
{
	m_CamController.Update(ts);

	// Despawn fallen objects
	for (uint32_t i = 0; i < Eis::PhysicsManager2D::GetBodyCount(); i++)
	{
		if (Eis::PhysicsManager2D::GetBody(i).GetPosition().y < -50.0f)
		{
			Eis::PhysicsManager2D::RemoveBody(i);
			m_Colors.erase(m_Colors.begin() + i);
		}
	}

	Eis::PhysicsManager2D::Update(15, ts);
}

void PhysicsDemo::Render()
{
	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());

	for (uint32_t i = 0; i < Eis::PhysicsManager2D::GetBodyCount(); i++)
		Eis::PhysicsManager2D::GetBody(i).Draw(m_Colors[i], m_DrawCircleLine);

	if (m_DrawBB)
		for (const auto& b : Eis::PhysicsManager2D::GetBodies())
			b.DrawBoundingBox();

	if (m_DrawVertices)
		for (const auto& b : Eis::PhysicsManager2D::GetBodies())
			b.DrawVertices();

	Eis::Renderer2D::EndScene();
}

void PhysicsDemo::ImGuiRender()
{
	ImGui::Begin("Info");

	ImGui::Text("LMB - Add box\nRMB - Add circle");

	ImGui::Text("Object count: %i", Eis::PhysicsManager2D::GetBodyCount());
	if (ImGui::Button("Clear"))
	{
		Eis::PhysicsManager2D::ClearBodies();
		Eis::PhysicsManager2D::AddBody(glm::vec2(0, -5.0f), 0.0f, glm::vec2(40.0f, 1.0f), 0, 0.3f, true);
		m_Colors.clear();
		m_Colors.emplace_back(glm::vec4(0.2f, 0.85f, 0.2f, 1.0f));
	}
	ImGui::Checkbox("Draw BBs", &m_DrawBB);
	ImGui::Checkbox("Draw vertices", &m_DrawVertices);
	ImGui::Checkbox("Draw circle lines", &m_DrawCircleLine);

	ImGui::End();
}

void PhysicsDemo::OnEvent(Eis::Event& e)
{
	Eis::EventDispatcher d(e);
	d.Dispatch<Eis::MouseButtonPressedEvent>([this](Eis::MouseButtonPressedEvent e) -> bool
	{
		if (ImGui::GetIO().WantCaptureMouse)
			return false;

		if (e.GetMouseButton() == EIS_MOUSE_BUTTON_0)
		{
			Eis::PhysicsManager2D::AddBody(m_CamController.CalculateMouseWorldPos(), 0.0f, Eis::Random::Vec2(0.7f, 1.5f), 1.0f, 0.3f);
			m_Colors.emplace_back(glm::vec4(Eis::Random::Vec3(), 1.0f));
		}
		else if (e.GetMouseButton() == EIS_MOUSE_BUTTON_1)
		{
			Eis::PhysicsManager2D::AddBody(m_CamController.CalculateMouseWorldPos(), Eis::Random::Float(0.3f, 0.7f), 1.0f, 0.3f);
			m_Colors.emplace_back(glm::vec4(Eis::Random::Vec3(), 1.0f));
		}

		return false;
	});

	m_CamController.OnEvent(e);
}