#include "RenderinExp.h"

#include <imgui.h>
#include <Eis/RenderingExp/ExpRenderer.h>
#include "Demos/Menu.h"


RenderinExp::RenderinExp()
	: Eis::Layer{ "Renderin" }
{
	m_Circle = Eis::Texture2D::Create("assets/textures/circle.png");
}

void RenderinExp::Attach()
{
	Eis::ExpRenderer::Init();
	Eis::ExpRenderer::SetClearColor(glm::vec4{0, 0, 0, 1});

	Eis::ExpRenderer::AddLight(Eis::ExpRenderer::Light{ glm::vec2{1}, glm::vec3{1.0f}, 3.0f, 1.0f, 0.5f });
	Eis::ExpRenderer::AddLight(Eis::ExpRenderer::Light{ glm::vec2{-1}, glm::vec3{1.0f}, 3.0f, 1.0f, 0.5f });
}

void RenderinExp::Detach()
{
	Eis::ExpRenderer::Shutdown();
}


void RenderinExp::Render()
{
	m_Cam.Update();

	Eis::ExpRenderer::BeginScene(m_Cam.GetCamera());

	Eis::ExpRenderer::DrawQuad(glm::vec2{0.0f}, glm::vec2{ 1.0f }, m_Circle);
	Eis::ExpRenderer::DrawQuad(m_Cam.CalculateMouseWorldPos(), glm::vec2{1.0f}, m_Circle);


	Eis::ExpRenderer::QuadDesc desc{
			glm::vec2{0.0f, -2.0f},
			glm::vec2{5.0f, 1.0f},
			0.0f,
			nullptr,
			glm::vec4{ 0.2f, 0.8f, 0.2f, 1.0f }
	};
	Eis::ExpRenderer::DrawQuad(desc);


	Eis::ExpRenderer::EndScene();
}


void RenderinExp::ImGuiRender()
{
	ImGuiMenu(Eis);

	ImGui::Begin("h");

	static float infl{ 1.0f };
	ImGui::SliderFloat("infl", &infl, 0.0f, 1.0f);
	Eis::ExpRenderer::TmpLi(infl);
	static glm::vec3 ambient{ 0.1f };
	ImGui::ColorEdit3("Ambient", (float*)&ambient);
	Eis::ExpRenderer::SetAmbientLight(ambient, 1.0f);

	ImGui::End();
}


void RenderinExp::OnEvent(Eis::Event& e)
{
	m_Cam.OnEvent(e);
}