#include "GameTheoryDemo.h"


GTDemo::GTDemo(const std::string& name) : Demo(name) {}

void GTDemo::OnAttach()
{
	Eis::Renderer2D::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	m_Simulation.Init();
	m_Simulation.SetMatrix(GameTheory::RewardMatrix({ 0.1f, 0.1f, 0.5f, 0.1f }, 1.0f));
	//   A   B
	// A 0.1 0.1
	// B 0.5 0.1

	m_yHistory.push_back((float)m_Simulation.GetTypeCount(GameTheory::A) / m_Simulation.GetSubjects().size());
	m_xHistory.push_back(0.0f);
}


void GTDemo::Update(Eis::TimeStep ts)
{
	EIS_PROFILE_FUNCTION();
	Eis::Renderer2D::Clear();

	lastTs = ts;
}

void GTDemo::ImGuiRender()
{
	ImGui::Begin(m_Name.c_str());

	// Info
	ImGui::Text("Step: %i", m_Simulation.GetStep());
	ImGui::Text("Subjects: %i", m_Simulation.GetSubjects().size());
	ImGui::Text("A Subjects: %i", m_Simulation.GetTypeCount(GameTheory::A));
	ImGui::Text("B Subjects: %i", m_Simulation.GetTypeCount(GameTheory::B));

	// Controls
	static int stepsLeft = 0; // non-blocking step calculation
	if (ImGui::Button("Step") || stepsLeft > 0)
	{
		m_Simulation.Step();
		stepsLeft--;

		m_yHistory.push_back((float)m_Simulation.GetTypeCount(GameTheory::A) / m_Simulation.GetSubjects().size());
		m_xHistory.push_back(m_Simulation.GetStep() / 10.0f);

		if (m_Simulation.GetTypeCount(GameTheory::A) == 0 || m_Simulation.GetTypeCount(GameTheory::A) == m_Simulation.GetSubjects().size())
			stepsLeft = 0;
	}
	if (ImGui::Button("10 Steps")) stepsLeft = 10;
	ImGui::SameLine();
	if (ImGui::Button("50 Steps")) stepsLeft = 50;
	if (ImGui::Button("Reset"))
	{
		stepsLeft = 0;
		m_Simulation.Reset();
		m_yHistory.clear();
		m_yHistory.push_back((float)m_Simulation.GetTypeCount(GameTheory::A) / m_Simulation.GetSubjects().size());
		m_xHistory.clear();
		m_xHistory.push_back(0.0f);
	}

	ImGui::InputFloat2("", (float*)&m_Simulation.GetMatrix().GetMatrix()[0]);
	ImGui::InputFloat2("", (float*)&m_Simulation.GetMatrix().GetMatrix()[1]);

	ImGui::NewLine();

	// Graph
	if (ImPlot::BeginPlot("##Ratio"))
	{
		ImPlot::SetupAxes("Step * 0.1", "Ratio");
		float i = 1.0f;
		ImPlot::PlotInfLines("", &i, 1, ImPlotInfLinesFlags_Horizontal);
		ImPlot::PlotShaded("B", m_xHistory.data(), m_yHistory.data(), (int)m_yHistory.size(), 1.0);
		ImPlot::PlotShaded("A", m_xHistory.data(), m_yHistory.data(), (int)m_yHistory.size());
		ImPlot::EndPlot();
	}

	ImGui::End();
}