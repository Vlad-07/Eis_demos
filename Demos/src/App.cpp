#include <Eis/Core/Application.h>
#include <Eis/Core/EntryPoint.h>


#include "Demos/Overview/OverviewDemo.h"
#include "Demos/Physics/PhysicsDemo.h"
#include "Demos/Sand/SandDemo.h"
#include "Demos/Chat/ChatDemo.h"
#include "Demos/Renderin/RenderinExp.h"


class App : public Eis::Application
{
public:
	App()
	{
		RegisterLayer<RenderinExp>("Renderin");
		RegisterLayer<OverviewDemo>("Overview");
		RegisterLayer<PhysicsDemo>("Physics");
		RegisterLayer<SandDemo>("Sand");
#ifdef EIS_NETWORKING_ENABLE
		RegisterLayer<ChatDemo>("Chat");
#endif
	}

	virtual ~App() = default;
};

Eis::Application* Eis::CreateApplication()
{
	return new App();
}