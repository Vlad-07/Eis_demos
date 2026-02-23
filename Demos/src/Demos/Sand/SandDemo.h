#pragma once

#include <Eis.h>
#include <imgui.h>

#include "World.h"


class SandDemo : public Eis::Layer
{
public:
	SandDemo();
	virtual ~SandDemo() = default;

	virtual void Attach() override;

	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void ImGuiRender() override;

	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController{};
	World m_World;
	ElementParams m_BrushElement = ElementParams::ID::SAND;
	float m_BrushSize = 1.0f;

	bool m_BrushPressed = false;
	bool m_EraserPressed = false;
	glm::uvec2 m_LastBrushPos{};
	glm::uvec2 m_LastEraserPos{};

private:
	static constexpr glm::uvec2 c_WorldSize = glm::vec2(302, 188);
	static constexpr glm::vec2 c_CellSize = glm::vec2(0.1f);
};