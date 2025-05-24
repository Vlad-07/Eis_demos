#pragma once

#include <Eis/Core/Core.h>

#ifdef EIS_NETWORKING_ENABLE

#include <Eis.h>
#include <imgui.h>

#include "Demos/Demo.h"

#include <Eis/Networking/Server.h>
#include <Eis/Networking/Client.h>

#include "ChatHelper.h"


// Chat app demo
// It's purpose is to showcase the networing capabilities
// of Eis. From an application standpoint, it's terrible
// Should be rewritten to demonstrate an actual architecture

class ChatDemo : public Demo
{
public:
	ChatDemo(const std::string& name);
	virtual ~ChatDemo() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;

private:
	Eis::Scope<Eis::Server> m_Server; // Normally you should write sepparate
	Eis::Scope<Eis::Client> m_Client; // apps for the server and the client
	ChatHelper m_ChatHelper;

	struct Config
	{
		bool isServer, isClient;
		uint32_t port;
		char ip[32];
	} m_Conf;
	char m_MessageBuf[512]{};
};

#endif