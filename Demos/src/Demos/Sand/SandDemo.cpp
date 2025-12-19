#include "SandDemo.h"

#include "Demos/Menu.h"


SandDemo::SandDemo(const std::string& name)
	: Layer(name), m_World(c_WorldSize)
{}

void SandDemo::Attach()
{
	Eis::Renderer2D::SetClearColor(glm::vec3(0.0f));
	m_CamController.SetMaxZoom(100.0f);
	m_CamController.SetZoom(10.5f);
	m_CamController.SetPosition(glm::vec2(c_WorldSize) * (c_CellSize.x / 2.0f) + glm::vec2(-2.5f, 0.0f));
}


void SandDemo::Update()
{
	EIS_PROFILE_FUNCTION();

	m_CamController.Update();

	// User Input
	glm::uvec2 mousePos = m_CamController.CalculateMouseWorldPos() * 10.0f + glm::vec2(0.5f);
	bool mouseInBounds = mousePos.x < c_WorldSize.x && mousePos.y < c_WorldSize.y;

	// Brush
	if (Eis::Input::IsMouseButtonPressed(EIS_MOUSE_BUTTON_0) && !ImGui::GetIO().WantCaptureMouse)
	{
		if (mouseInBounds && m_World.At(mousePos).Id == ElementParams::ID::AIR)
			m_World.SetElement(mousePos, m_BrushElement);

		m_BrushPressed = true;
		m_LastBrushPos = mousePos;
	}
	else
		m_BrushPressed = false;

	// Eraser
	if (!m_BrushPressed && Eis::Input::IsMouseButtonPressed(EIS_MOUSE_BUTTON_1))
	{
		if (mouseInBounds)
			m_World.SetElement(mousePos, ElementParams());

		m_EraserPressed = true;
		m_LastEraserPos = mousePos;
	}
	else
		m_EraserPressed = false;


	// Rendering
	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());

	// Background
	Eis::Renderer2D::DrawQuad(glm::vec2(c_WorldSize) * (c_CellSize.x / 2.0f) - glm::vec2(c_CellSize / 2.0f), m_World.GetSize() * c_CellSize, g_Elements[ElementParams::AIR].Color);

	// Cells
	for (uint32_t y = 0; y < c_WorldSize.y; y++)
	for (uint32_t x = 0; x < c_WorldSize.x; x++)
		if (m_World.At(x, y) != ElementParams::AIR)
			Eis::Renderer2D::DrawQuad(glm::vec2(x, y) * c_CellSize, c_CellSize, g_Elements[m_World.At(x, y)].Color);

	// Cell highlight
	if (mouseInBounds)
		Eis::Renderer2D::DrawQuad(glm::vec2(mousePos) * c_CellSize, c_CellSize, glm::vec4(0.7f));

	Eis::Renderer2D::EndScene();
}

void SandDemo::FixedUpdate()
{
	m_World.Update();
}

void SandDemo::ImGuiRender()
{
	if (ImGui::Begin(m_Name.c_str())) {
		// Brushes
		for (uint8_t i = 0; i < g_Elements.size(); i++)
		{
			char name[32]{};
			if (g_Elements[m_BrushElement] == g_Elements[i])
				name[0] = '*', strcpy(name + 1, g_Elements[i].Name);
			else
				strcpy(name, g_Elements[i].Name);

			if (ImGui::Button(name))
				m_BrushElement.Id = (ElementParams::ID)i;
		}

		ImGui::Separator();

		if (ImGui::Button("Clear"))
			m_World.Clear();
	} ImGui::End();

	ImGuiMenu();
}

void SandDemo::OnEvent(Eis::Event& e)
{
	m_CamController.OnEvent(e);
}


Eis::Layer::Factory SandDemo::GetFactory()
{
	return [](const std::string& name) -> Eis::Scope<Layer> { return Eis::CreateScope<SandDemo>(name); };
}