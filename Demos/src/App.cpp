#include <Eis.h>
#include <Eis/Core/EntryPoint.h>

#include <imgui.h>

#include "DemoLayer.h"


class App : public Eis::Application
{
public:
	App()
	{
		PushLayer(new DemoLayer());
	}

	virtual ~App() = default;
};

Eis::Application* Eis::CreateApplication()
{
	return new App();
}