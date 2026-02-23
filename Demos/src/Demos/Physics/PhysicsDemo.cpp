#include "PhysicsDemo.h"

#include "Demos/Menu.h"


PhysicsDemo::PhysicsDemo() : Layer{ "Physics" }
{
	m_CamController.SetPoseLock(true);
	m_CamController.SetMaxZoom(100);
	m_CamController.SetZoom(15.0f);

	m_Entities.reserve(41);
	m_Entities.emplace_back(glm::vec2{ 0, -5.0f }, glm::vec2{ 60.0f, 1.0f }, glm::vec3{ 0.2f, 0.85f, 0.2f }, true);
	for (int i = 0; i < 40; i++)
	{
		if (i % 2)
			m_Entities.emplace_back(glm::vec2{ Eis::Random::Float(-15.0f, 15.0f), Eis::Random::Float(3.0f, 5.0f) }, Eis::Random::Float(0.3f, 0.7f), Eis::Random::Vec3());
		else
			m_Entities.emplace_back(glm::vec2{ Eis::Random::Float(-15.0f, 15.0f), Eis::Random::Float(3.0f, 7.0f) }, Eis::Random::Vec2(0.7f, 1.3f), Eis::Random::Vec3());
	}
}


void PhysicsDemo::Attach()
{
	Eis::Renderer2D::SetClearColor(glm::vec3(0.1f));
	Eis::Renderer2D::SetLineWidth(1.0f);
	Eis.Window().SetVSync(false);
}

void PhysicsDemo::Detach()
{
	Eis.Window().SetVSync(true);
}


void PhysicsDemo::FixedUpdate()
{
	// Despawn fallen objects
	for (size_t i{}; i < m_Entities.size(); i++)
	{
		if (m_Entities[i].GetPosition().y < -50.0f)
		{
			m_Entities.erase(m_Entities.begin() + i);
			i--;
		}
	}

	Eis::PhysicsManager2D::Update(20);
}

void PhysicsDemo::Update()
{
	m_CamController.Update();
}

void PhysicsDemo::Render()
{
	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());

	// not ideal rendering
	for (size_t i{}; i < m_Entities.size(); i++)
	{
		const Eis::Rigidbody2D& rb = m_Entities[i].GetRB();
		switch (rb.GetCollider().GetType())
		{
		case Eis::Collider2D::Type::CIRCLE:
		{
			const auto& c = rb.GetCollider().As<Eis::CircleCollider2D>();
			Eis::Renderer2D::DrawCircle(rb.GetPosition(), c.GetRadius() * 2.0f, glm::vec4{ m_Entities[i].GetColor(), 1.0f });
			if (m_DrawCircleLine)
				Eis::Renderer2D::DrawLine(rb.GetPosition(), rb.GetRotation(), c.GetRadius(), glm::vec4{ 0, 0, 0, 1 }); // Rotation visualiser
			break;
		}

		case Eis::Collider2D::Type::POLYGON:
			Eis::Renderer2D::DrawQuad(*(glm::mat4x2*)rb.GetCollider().As<Eis::PolygonCollider2D>()
				.GetTransformedVertices(rb.GetPosition(), rb.GetRotation()).data(), glm::vec4{ m_Entities[i].GetColor(), 1.0f });
			break;

		default:
			EIS_CORE_ERROR("Invalid Rigidbody2D type!");
		}

	}

	if (m_DrawBB)
		for (const auto& e : m_Entities)
		{
			const Eis::BBox2D& bb = e.GetRB().GetBBox();
			Eis::Renderer2D::DrawLine(bb.BottomLeft, { bb.TopRight.x, bb.BottomLeft.y }, glm::vec4(0, 1, 0, 1));
			Eis::Renderer2D::DrawLine({ bb.TopRight.x, bb.BottomLeft.y }, bb.TopRight, glm::vec4(0, 1, 0, 1));
			Eis::Renderer2D::DrawLine(bb.TopRight, { bb.BottomLeft.x, bb.TopRight.y }, glm::vec4(0, 1, 0, 1));
			Eis::Renderer2D::DrawLine({ bb.BottomLeft.x, bb.TopRight.y }, bb.BottomLeft, glm::vec4(0, 1, 0, 1));
		}

	if (m_DrawVertices)
		for (const auto& e : m_Entities)
		{
			const Eis::Rigidbody2D& rb = e.GetRB();
			if (rb.GetCollider().GetType() != Eis::Collider2D::Type::POLYGON)
				continue;

			const auto& tv = rb.GetCollider().As<Eis::PolygonCollider2D>().GetTransformedVertices(rb.GetPosition(), rb.GetRotation());
			Eis::Renderer2D::DrawCircle(tv[0], glm::vec2(0.1f), glm::vec4(1.0f));
			Eis::Renderer2D::DrawCircle(tv[1], glm::vec2(0.1f), glm::vec4(1.0f));
			Eis::Renderer2D::DrawCircle(tv[2], glm::vec2(0.1f), glm::vec4(1.0f));
			Eis::Renderer2D::DrawCircle(tv[3], glm::vec2(0.1f), glm::vec4(1.0f));
		}

	Eis::Renderer2D::EndScene();
}

void PhysicsDemo::ImGuiRender()
{
	ImGui::Begin("Info");

	ImGui::Text("LMB - Add box\nRMB - Add circle");

	ImGui::Text("Object count: %i", Eis::PhysicsManager2D::GetBodyCount());
	if (ImGui::Button("Clear"))
	{
		m_Entities.clear();
		m_Entities.emplace_back(glm::vec2(0, -5.0f), glm::vec2(60.0f, 1.0f), glm::vec3{ 0.2f, 0.85f, 0.2f }, true);
	}
	ImGui::Checkbox("Draw BBs", &m_DrawBB);
	ImGui::Checkbox("Draw vertices", &m_DrawVertices);
	ImGui::Checkbox("Draw circle lines", &m_DrawCircleLine);

	ImGui::End();

	ImGuiMenu(Eis);
}

void PhysicsDemo::OnEvent(Eis::Event& e)
{
	Eis::EventDispatcher d(e);
	d.Dispatch<Eis::KeyPressedEvent>([&](Eis::KeyPressedEvent& e) -> bool
		{
			if (e.GetKeyCode() == EIS_KEY_E)
				m_Entities.emplace_back(m_CamController.CalculateMouseWorldPos(), Eis::Random::Vec2(0.7f, 1.5f), Eis::Random::Vec3());

			return false;
		});
	d.Dispatch<Eis::MouseButtonPressedEvent>([&](Eis::MouseButtonPressedEvent& e) -> bool
		{
			if (ImGui::GetIO().WantCaptureMouse)
				return false;

			if (e.GetMouseButton() == EIS_MOUSE_BUTTON_0)
				m_Entities.emplace_back(m_CamController.CalculateMouseWorldPos(), Eis::Random::Vec2(0.7f, 1.5f), Eis::Random::Vec3());

			else if (e.GetMouseButton() == EIS_MOUSE_BUTTON_1)
				m_Entities.emplace_back(m_CamController.CalculateMouseWorldPos(), Eis::Random::Float(0.7f, 1.5f), Eis::Random::Vec3());

			return false;
		});

	m_CamController.OnEvent(e);
}