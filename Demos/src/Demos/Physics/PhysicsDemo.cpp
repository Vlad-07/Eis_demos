#include "PhysicsDemo.h"

#include "Demos/Menu.h"


PhysicsDemo::PhysicsDemo(const std::string& name) : Layer(name)
{
	m_CamController.SetPoseLock(true);
	m_CamController.SetMaxZoom(100);
	m_CamController.SetZoom(15.0f);

	Eis::PhysicsManager2D::AddBody(glm::vec2(0, -5.0f), 0.0f, glm::vec2(60.0f, 1.0f), 0, 0.3f, true);
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
	Eis.Window().SetVSync(false);
}

void PhysicsDemo::Detach()
{
	Eis::PhysicsManager2D::ClearBodies();
	Eis.Window().SetVSync(true);
}


void PhysicsDemo::FixedUpdate()
{
	// Despawn fallen objects
	for (uint32_t i = 0; i < Eis::PhysicsManager2D::GetBodyCount(); i++)
	{
		if (Eis::PhysicsManager2D::GetBody(i).GetPosition().y < -50.0f)
		{
			Eis::PhysicsManager2D::RemoveBody(i);
			m_Colors.erase(m_Colors.begin() + i);
		}
	}

	Eis::PhysicsManager2D::Update(10);
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
	for (uint32_t i = 0; i < Eis::PhysicsManager2D::GetBodyCount(); i++)
	{
		const Eis::Rigidbody2D& rb = Eis::PhysicsManager2D::GetBody(i);
		switch (rb.GetCollider().GetType())
		{
		case Eis::Collider2D::Type::CIRCLE:
		{
			const auto& c = rb.GetCollider().As<Eis::CircleCollider2D>();
			Eis::Renderer2D::DrawCircle(rb.GetPosition(), c.GetRadius() * 2.0f, m_Colors[i]);
			if (m_DrawCircleLine)
				Eis::Renderer2D::DrawLine(rb.GetPosition(), glm::degrees(rb.GetRotation()), c.GetRadius(), glm::vec4(0, 0, 0, 1)); // Rotation visualiser
			break;
		}

		case Eis::Collider2D::Type::POLYGON:
			Eis::Renderer2D::DrawQuad(*(glm::mat4x2*)rb.GetCollider().As<Eis::PolygonCollider2D>().GetTransformedVertices(rb.GetPosition(), rb.GetRotation()).data(), m_Colors[i]);
			break;

		default:
			EIS_CORE_ERROR("Invalid Rigidbody2D type!");
		}

	}

	if (m_DrawBB)
		for (const auto& rb : Eis::PhysicsManager2D::GetBodies())
		{
			const Eis::BBox2D& bb = rb.GetBBox();
			Eis::Renderer2D::DrawLine(bb.BottomLeft, { bb.TopRight.x, bb.BottomLeft.y }, glm::vec4(0, 1, 0, 1));
			Eis::Renderer2D::DrawLine({ bb.TopRight.x, bb.BottomLeft.y }, bb.TopRight, glm::vec4(0, 1, 0, 1));
			Eis::Renderer2D::DrawLine(bb.TopRight, { bb.BottomLeft.x, bb.TopRight.y }, glm::vec4(0, 1, 0, 1));
			Eis::Renderer2D::DrawLine({ bb.BottomLeft.x, bb.TopRight.y }, bb.BottomLeft, glm::vec4(0, 1, 0, 1));
		}

	if (m_DrawVertices)
		for (const auto& rb : Eis::PhysicsManager2D::GetBodies())
		{
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
		Eis::PhysicsManager2D::ClearBodies();
		Eis::PhysicsManager2D::AddBody(glm::vec2(0, -5.0f), 0.0f, glm::vec2(60.0f, 1.0f), 0, 0.3f, true);
		m_Colors.clear();
		m_Colors.emplace_back(glm::vec4(0.2f, 0.85f, 0.2f, 1.0f));
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
			{
				Eis::PhysicsManager2D::AddBody(m_CamController.CalculateMouseWorldPos(), 0.0f, Eis::Random::Vec2(0.7f, 1.5f), 1.0f, 0.3f);
				m_Colors.emplace_back(glm::vec4(Eis::Random::Vec3(), 1.0f));
			}
			return false;
		});
	d.Dispatch<Eis::MouseButtonPressedEvent>([&](Eis::MouseButtonPressedEvent& e) -> bool
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


Eis::Layer::Factory PhysicsDemo::GetFactory()
{
	return [](const std::string& name) -> Eis::Scope<Layer> { return Eis::CreateScope<PhysicsDemo>(name); };
}