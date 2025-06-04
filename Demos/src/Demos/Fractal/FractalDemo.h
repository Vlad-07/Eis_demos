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
	uint32_t m_MaxIt = 500;

	inline static const std::array s_FractalLib{
		std::make_pair(glm::vec2(-0.209f, 0.696f), 500),
		std::make_pair(glm::vec2(-0.4f, 0.6f), 200),
		std::make_pair(glm::vec2(-0.02f, 0.79f), 150),
		std::make_pair(glm::vec2(-0.8f, 0.156f), 500),
		std::make_pair(glm::vec2(0.35f, 0.358f), 500),
		std::make_pair(glm::vec2(-0.21f, 0.79f), 400),
		std::make_pair(glm::vec2(0.407f, 0.306f), 100),
		std::make_pair(glm::vec2(-1.166f, -0.259f), 100)
	};

private:
	static constexpr uint32_t c_DroneNr = 10;
};