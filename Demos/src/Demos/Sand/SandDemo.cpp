#include "SandDemo.h"

SandDemo::SandDemo(const std::string& name) : Demo(name), m_CamController(16.0f / 9.0f), m_BrushElement(Element()) {}

void SandDemo::OnAttach()
{
	Eis::Renderer2D::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
}


void SandDemo::Update(Eis::TimeStep ts)
{
	m_CamController.OnUpdate(ts);

	if (Eis::Input::IsMouseButtonPressed(EIS_MOUSE_BUTTON_0)) // Brush
	{
		glm::vec<2, uint32_t> pos = m_CamController.CalculateMouseWorldPos() * 10.0f + glm::vec2(0.5f);

		if (pos.x < m_World.GetSize().x && pos.y < m_World.GetSize().y)
			m_World.SetElement(pos, m_BrushElement);
	}
	else if (Eis::Input::IsMouseButtonPressed(EIS_MOUSE_BUTTON_1)) // Eraser
	{
		glm::vec<2, uint32_t> pos = m_CamController.CalculateMouseWorldPos() * 10.0f + glm::vec2(0.5f);

		if (pos.x < m_World.GetSize().x && pos.y < m_World.GetSize().y)
			m_World.SetElement(pos, Element::AIR);
	}

	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());
	for (uint32_t y = 0; y < m_World.GetSize().y; y++)
	for (uint32_t x = 0; x < m_World.GetSize().x; x++)
		Eis::Renderer2D::DrawQuad({ x / 10.0f, y / 10.0f }, glm::vec2(0.1f), glm::vec4(Elements[(int8_t)m_World.At(x, y)].Color, 1.0f));

	Eis::Renderer2D::EndScene();

	static auto lastUpdateTime = std::chrono::high_resolution_clock::now();
	if (std::chrono::high_resolution_clock::now() >= lastUpdateTime + std::chrono::milliseconds(16))
		m_World.Update(), lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void SandDemo::ImGuiRender()
{
	ImGui::Begin(m_Name.c_str());

	if (ImGui::Button("Air"))	m_BrushElement = Element::AIR;
	if (ImGui::Button("Sand"))	m_BrushElement = Element::SAND;
	if (ImGui::Button("Water"))	m_BrushElement = Element::WATER;
	if (ImGui::Button("Stone"))	m_BrushElement = Element::STONE;

	ImGui::Separator();

	if (ImGui::Button("Clear")) m_World.Clear();

	ImGui::End();
}

void SandDemo::OnEvent(Eis::Event& e)
{
	m_CamController.OnEvent(e);
}