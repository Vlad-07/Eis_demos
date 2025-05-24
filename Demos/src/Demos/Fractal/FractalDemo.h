#pragma once

#include <Eis.h>
#include <imgui.h>

#include "Demos/Demo.h"
#include "FractalRenderer.h"


class FractalDemo : public Demo
{
public:
	FractalDemo(const std::string& name);
	virtual ~FractalDemo() = default;

	virtual void OnAttach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController;
	FractalRenderer m_FractalRenderer;

	glm::vec2 m_Constant = glm::vec2(-0.209f, 0.696f);
	int m_MaxIt = 500;
	bool m_Auto = false;

private:
	static constexpr uint32_t c_DroneNr = 10;
};