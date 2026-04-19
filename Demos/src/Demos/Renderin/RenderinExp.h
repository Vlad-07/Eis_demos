#pragma once

#include <Eis.h>


class RenderinExp : public Eis::Layer
{
public:
	RenderinExp();
	virtual ~RenderinExp() = default;

	void Attach() override;
	void Detach() override;

	void Render() override;
	void ImGuiRender() override;
	void OnEvent(Eis::Event& e);

private:
	Eis::OrthoCameraController m_Cam{};
	Eis::Ref<Eis::Texture2D> m_Circle;
};