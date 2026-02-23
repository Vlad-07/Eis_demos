#pragma once

#include <Eis.h>
#include <imgui.h>


// Demos landing. Whacky conglomerate of everything

class OverviewDemo : public Eis::Layer
{
public:
	OverviewDemo();
	virtual ~OverviewDemo() = default;

	virtual void Attach() override;
	virtual void Detach() override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void ImGuiRender() override;

	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CameraController{};
	Eis::Ref<Eis::Texture2D> ice, mouce;
	float m_LineAngle{}, m_CircleThickness{0.9f}, m_CircleFade{0.1f};
};