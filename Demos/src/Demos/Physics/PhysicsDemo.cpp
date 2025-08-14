#include "PhysicsDemo.h"


PhysicsDemo::PhysicsDemo(const std::string& name) : Demo(name)
{
	m_CamController.SetMaxZoom(100);

	Eis::PhysicsManager2D::AddBody(glm::vec2(0, -5.0f), 0.0f, glm::vec2(40.0f, 1.0f), 0, 0.8f, true);
	for (int i = 0; i < 10; i++)
	{
		if (i % 2)
			Eis::PhysicsManager2D::AddBody(Eis::Random::Vec2(-1.0f, 5.0f), 1, 1.0f, 0.8f);
		else
			Eis::PhysicsManager2D::AddBody(Eis::Random::Vec2(-1.0f, 5.0f), 0, Eis::Random::Vec2(0.7f, 1.3f), 1.0f, 0.8f);
	}
}

void PhysicsDemo::OnAttach()
{
	Eis::Renderer2D::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void PhysicsDemo::Update(Eis::TimeStep ts)
{
	// Compute
	m_CamController.OnUpdate(ts);

	Eis::PhysicsManager2D::GetBody(1).MoveTo(m_CamController.CalculateMouseWorldPos());

	

	// Rendering
	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());

	Eis::PhysicsManager2D::Update(1, ts);

	for (int i = 0; i < Eis::PhysicsManager2D::GetBodies().size(); i++)
	{
		const auto& b = Eis::PhysicsManager2D::GetBodies()[i];
		b.Draw(glm::vec4(0.85f, i == 2 ? 0 : 0.85f, 0.85f, 1.0f));
	}
	

	/* Debug vertices
	for (Eis::Rigidbody2D& b : Eis::PhysicsManager2D::GetBodies())
	{
		auto& h = b.GetTransformedVertices();
		Eis::Renderer2D::DrawCircle(h[0], glm::vec2(0.1f), glm::vec4(1.0f));
		Eis::Renderer2D::DrawCircle(h[1], glm::vec2(0.1f), glm::vec4(1.0f));
		Eis::Renderer2D::DrawCircle(h[2], glm::vec2(0.1f), glm::vec4(1.0f));
		Eis::Renderer2D::DrawCircle(h[3], glm::vec2(0.1f), glm::vec4(1.0f));
	}//*/

	Eis::Renderer2D::EndScene();
}

void PhysicsDemo::ImGuiRender()
{
	ImGui::Begin("Info");

	int id = 1;
	ImGui::Text("Obj %i", id);
	ImGui::Text("Pos %.2f %.2f", Eis::PhysicsManager2D::GetBodies()[id].GetPosition().x, Eis::PhysicsManager2D::GetBodies()[id].GetPosition().y);
	ImGui::Text("Vel %.2f %.2f", Eis::PhysicsManager2D::GetBodies()[id].GetLinearVelocity().x, Eis::PhysicsManager2D::GetBodies()[id].GetLinearVelocity().y);

	ImGui::End();
}

void PhysicsDemo::OnEvent(Eis::Event& e)
{
	Eis::EventDispatcher d(e);
	d.Dispatch<Eis::MouseButtonPressedEvent>([this](Eis::MouseButtonPressedEvent e) -> bool
	{
		if (e.GetMouseButton() == EIS_MOUSE_BUTTON_0)
			Eis::PhysicsManager2D::AddBody(m_CamController.CalculateMouseWorldPos(), 0.0f, Eis::Random::Vec2(0.7f, 1.5f), 1.0f, 0.0f);
		else if (e.GetMouseButton() == EIS_MOUSE_BUTTON_1)
			Eis::PhysicsManager2D::AddBody(m_CamController.CalculateMouseWorldPos(), Eis::Random::Float(0.7f, 1.5f), 1.0f, 0.0f);
		return false;
	});

	m_CamController.OnEvent(e);
}