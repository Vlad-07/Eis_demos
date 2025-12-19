#include <Eis/Core/Application.h>
#include <Eis/Core/EntryPoint.h>

#include "DemoLayer.h"


class App : public Eis::Application
{
public:
	App()
	{
		PushLayer(Eis::CreateScope<DemoLayer>());
	}

	virtual ~App() = default;
};

Eis::Application* Eis::CreateApplication()
{
	return new App();
}