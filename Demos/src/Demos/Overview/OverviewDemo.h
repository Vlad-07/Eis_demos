#pragma once

#include <Eis.h>
#include <imgui.h>
#include <implot.h>

#include "Demos/Demo.h"

// Demos landing. Whacky conglomerate of everything

class OverviewDemo : public Demo
{
public:
	OverviewDemo(const std::string& name);
	virtual ~OverviewDemo() = default;

	virtual void OnAttach() override;
	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CameraController;
	Eis::Ref<Eis::Texture2D> ice, mouce;
	float m_LineAngle, m_CircleThickness, m_CircleFade;
};