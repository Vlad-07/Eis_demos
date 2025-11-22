#pragma once

#include <Eis.h>
#include <imgui.h>

#include "Demos/Demo.h"
#include "World.h"


class SandDemo : public Demo
{
public:
	SandDemo(const std::string& name);
	virtual ~SandDemo() = default;

	virtual void Attach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;

	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController;
	World m_World;
	ElementParams m_BrushElement;
	float m_BrushSize = 1.0f;

	// TODO: implement line brush when drag clicking
	bool m_BrushPressed = false;
	bool m_EraserPressed = false;
	glm::ivec2 m_LastBrushPos{};
	glm::ivec2 m_LastEraserPos{};

	float m_LastComputeTime = 0;

private:
	static constexpr glm::vec<2, uint32_t> c_WorldSize = glm::vec2(302, 188);
	static constexpr glm::vec2 c_CellSize = glm::vec2(0.1f);
};