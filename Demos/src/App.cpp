#include <Eis/Core/Application.h>
#include <Eis/Core/EntryPoint.h>


#include "Demos/Overview/OverviewDemo.h"
#include "Demos/Physics/PhysicsDemo.h"
#include "Demos/Sand/SandDemo.h"
#include "Demos/Chat/ChatDemo.h"


class App : public Eis::Application
{
public:
	App()
	{
		RegisterLayer(OverviewDemo::GetFactory(), "Overview");
		RegisterLayer(PhysicsDemo::GetFactory(), "Physics");
		RegisterLayer(SandDemo::GetFactory(), "Sand");
#ifdef EIS_NETWORKING_ENABLE
		RegisterLayer(ChatDemo::GetFactory(), "Chat");
#endif
	}

	virtual ~App() = default;
};

Eis::Application* Eis::CreateApplication()
{
	return new App();
}