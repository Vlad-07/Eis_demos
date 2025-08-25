#include "PhysicsDemo.h"


PhysicsDemo::PhysicsDemo(const std::string& name) : Demo(name)
{
	m_CamController.SetPoseLock(true);
	m_CamController.SetMaxZoom(100);

	Eis::PhysicsManager2D::AddBody(glm::vec2(0, -5.0f), 0.0f, glm::vec2(40.0f, 1.0f), 0, 0.3f, true);
	//for (int i = 0; i < 20; i++)
	{
	//	if (i % 2)
	//		Eis::PhysicsManager2D::AddBody(Eis::Random::Vec2(-1.0f, 5.0f), Eis::Random::Float(0.3f, 0.7f), 1.0f, 0.5f);
	//	else
	//		Eis::PhysicsManager2D::AddBody({ Eis::Random::Float(-5.0f, 5.0f), Eis::Random::Float(0, 50) }, 0, Eis::Random::Vec2(0.7f, 1.3f), 1.0f, 0.5f);
	}
}

void PhysicsDemo::OnAttach()
{
	Eis::Renderer2D::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Eis::Renderer2D::SetLineWidth(1.0f);
	Eis::Application::Get().GetWindow().SetVSync(false);
}

void PhysicsDemo::Update(Eis::TimeStep ts)
{
	// Input
	//Eis::PhysicsManager2D::GetBody(1).MoveTo(m_CamController.CalculateMouseWorldPos());

	/*if (Eis::Input::IsKeyPressed(EIS_KEY_W))
		Eis::PhysicsManager2D::GetBody(1).AddForce({ 0.0f, 10.0f });
	if (Eis::Input::IsKeyPressed(EIS_KEY_S))
		Eis::PhysicsManager2D::GetBody(1).AddForce({ 0.0f,-10.0f });
	if (Eis::Input::IsKeyPressed(EIS_KEY_A))
		Eis::PhysicsManager2D::GetBody(1).AddForce({-10.0f, 0.0f });
	if (Eis::Input::IsKeyPressed(EIS_KEY_D))
		Eis::PhysicsManager2D::GetBody(1).AddForce({ 10.0f, 0.0f });
	if (Eis::Input::IsKeyPressed(EIS_KEY_Q))
		Eis::PhysicsManager2D::GetBody(1).Rotate(1.0f);
	if (Eis::Input::IsKeyPressed(EIS_KEY_E))
		Eis::PhysicsManager2D::GetBody(1).Rotate(-1.0f);*/

	// Compute
	m_CamController.OnUpdate(ts);

	// Despawn fallen objects
	for (uint32_t i = 0; i < Eis::PhysicsManager2D::GetBodyCount(); i++)
	{
		if (Eis::PhysicsManager2D::GetBody(i).GetPosition().y < -50.0f)
			Eis::PhysicsManager2D::RemoveBody(i);
	}


	Eis::PhysicsManager2D::Update(20, ts);


	// Rendering
	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());

	for (const auto& b : Eis::PhysicsManager2D::GetBodies())
		b.Draw(glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));

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

	ImGui::Text("Object count: %i", Eis::PhysicsManager2D::GetBodyCount());
	if (ImGui::Button("Clear"))
	{
		Eis::PhysicsManager2D::ClearBodies();
		Eis::PhysicsManager2D::AddBody(glm::vec2(0, -5.0f), 0.0f, glm::vec2(40.0f, 1.0f), 0, 0.3f, true);
	}
	ImGui::Checkbox("Draw BBs", &m_DrawBB);
	ImGui::Checkbox("Draw vertices", &m_DrawVertices);

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
			Eis::PhysicsManager2D::AddBody(m_CamController.CalculateMouseWorldPos(), 0.0f, /*Eis::Random::Vec2(0.7f, 1.5f)*/glm::vec2(1), 1.0f, 0.3f);
		else if (e.GetMouseButton() == EIS_MOUSE_BUTTON_1)
			Eis::PhysicsManager2D::AddBody(m_CamController.CalculateMouseWorldPos(), Eis::Random::Float(0.3f, 0.7f), 1.0f, 0.3f);

		return false;
	});

	m_CamController.OnEvent(e);
}