#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Eis.h>
#include <imgui.h>

#include "Demos/Demo.h"
#include "World.h"


class SandDemo : public Demo
{
public:
	SandDemo(const std::string& name);
	~SandDemo() = default;

	virtual void OnAttach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	const glm::vec<2, uint32_t> c_WorldSize = glm::vec2(302, 188);
	const glm::vec2 c_CellSize = glm::vec2(0.1f);

private:
	Eis::OrthoCameraController m_CamController;
	World m_World;
	ElementParams m_BrushElement;
	float m_BrushSize;

	// TODO: implement line brush when drag clicking
	bool m_BrushPressed{};
	bool m_EraserPressed{};
	glm::ivec2 m_LastBrushPos{};
	glm::ivec2 m_LastEraserPos{};


	float m_LastComputeTime{};
};