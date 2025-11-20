#pragma once

#include <Eis.h>
#include <imgui.h>
#include <implot.h>

#include "DemoManager.h"
#include "Demos/Overview/OverviewDemo.h"
#include "Demos/Physics/PhysicsDemo.h"
#include "Demos/Sand/SandDemo.h"
#include "Demos/Chat/ChatDemo.h"


// Various demos, mini-projects I worked on,
// demonstrating the capabilities of Eis and act like 'unit tests'.

class DemoLayer : public Eis::Layer
{
public:
	DemoLayer();
	virtual ~DemoLayer() = default;

	virtual void Attach() override;
	virtual void Detach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	DemoManager m_DemoManager;
	Eis::TimeStep m_LastTs;
};