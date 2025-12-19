#pragma once

#include <Eis.h>
#include <Eis/Physics/Physics.h>
#include <imgui.h>


// Showcase of the physics 'engine'
// Should have used Box2D

class PhysicsDemo : public Eis::Layer
{
public:
	PhysicsDemo(const std::string& name);
	virtual ~PhysicsDemo() = default;

	virtual void Attach() override;
	virtual void Detach() override;

	virtual void Update() override;
	virtual void FixedUpdate() override;

	virtual void Render() override;
	virtual void ImGuiRender() override;

	virtual void OnEvent(Eis::Event& e) override;

	static Factory GetFactory();

private:
	Eis::OrthoCameraController m_CamController;

	std::vector<glm::vec4> m_Colors; // no ecs so colors are stored in here
									 // should make a entity class (until ECS is implemented (far future))

	bool m_DrawVertices = false;
	bool m_DrawBB = false;
	bool m_DrawCircleLine = true;
};