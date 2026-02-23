#pragma once

#include <Eis.h>
#include <Eis/Physics/Physics.h>
#include <imgui.h>

#include "Entity.h"


// Showcase of the physics 'engine'
// Should have used Box2D

class PhysicsDemo : public Eis::Layer
{
public:
	PhysicsDemo();
	virtual ~PhysicsDemo() = default;

	virtual void Attach() override;
	virtual void Detach() override;

	virtual void Update() override;
	virtual void FixedUpdate() override;

	virtual void Render() override;
	virtual void ImGuiRender() override;

	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController;

	std::vector<Entity> m_Entities;

	bool m_DrawVertices = false;
	bool m_DrawBB = false;
	bool m_DrawCircleLine = true;
};