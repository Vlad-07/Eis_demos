#pragma once

#include <Eis.h>
#include <Eis/Physics/Physics.h>
#include <imgui.h>

#include "Demos/Demo.h"


class PhysicsDemo : public Demo
{
public:
	PhysicsDemo(const std::string& name);
	virtual ~PhysicsDemo() = default;

	virtual void OnAttach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController;

	bool m_DrawVertices = false;
	bool m_DrawBB = false;
};