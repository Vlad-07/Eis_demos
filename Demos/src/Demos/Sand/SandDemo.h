#pragma once

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
	Eis::OrthoCameraController m_CamController;
	World m_World;
	Element m_BrushElement;
};