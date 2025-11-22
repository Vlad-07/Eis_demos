#pragma once

#include <Eis.h>
#include <Eis/Physics/Physics.h>
#include <imgui.h>

#include "Demos/Demo.h"

// Showcase of the physics 'engine'
// Should have used Box2D

class PhysicsDemo : public Demo
{
public:
	PhysicsDemo(const std::string& name);
	virtual ~PhysicsDemo() = default;

	virtual void Attach() override;
	virtual void Detach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void Render() override;
	virtual void ImGuiRender() override;

	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController;

	std::vector<glm::vec4> m_Colors; // HACK: no ecs so colors are stored in here
									 // should make a entity class (until ECS is implemented)

	bool m_DrawVertices = false;
	bool m_DrawBB = false;
	bool m_DrawCircleLine = true;
};