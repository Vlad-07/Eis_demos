#include "Menu.h"

#include <imgui.h>
#include <Eis/Core/Application.h>
#include <Eis/Rendering/Renderer/Renderer2D.h>


void ImGuiMenu(Eis::EisContext& c)
{
	ImGui::Begin("Demo Menu");

	if (ImGui::Button("Overview"))
		c.App().QueueTransition("Overview");
	if (ImGui::Button("Physics"))
		c.App().QueueTransition("Physics");
	if (ImGui::Button("Sand"))
		c.App().QueueTransition("Sand");
#ifdef EIS_NETWORKING_ENABLE
	if (ImGui::Button("Chat"))
		c.App().QueueTransition("Chat");
	if (ImGui::Button("Quit"))
		c.App().ShouldClose();
#endif

	ImGui::End();


	ImGui::Begin("Performance");
	// TODO: a proper performance system
	static uint32_t frames = 0; frames++;
	static Eis::Duration elapsed; elapsed += (float)Eis::Time::GetUncappedDeltaTime();
	static float fps = 0.0f;
	if (elapsed.GetSeconds() > 0.25f)
		fps = frames / (float)elapsed.GetSeconds(), frames = 0, elapsed = Eis::Duration::FromSec(0.0f);

	ImGui::Text("%.1f FPS (%.3f ms)", fps, 1000.0f / fps);
	ImGui::Text("Draw calls:   %i", Eis::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quad count:   %i", Eis::Renderer2D::GetStats().QuadCount);
	ImGui::Text("Circle count: %i", Eis::Renderer2D::GetStats().CircleCount);
	ImGui::Text("Line count:   %i", Eis::Renderer2D::GetStats().LineCount);
	ImGui::End();
}