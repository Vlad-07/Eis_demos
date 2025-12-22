#include "ChatDemo.h"

#include "Demos/Menu.h"


#ifdef EIS_NETWORKING_ENABLE

ChatDemo::ChatDemo(const std::string& name) : Layer(name) {}


void ChatDemo::Attach()
{
	Eis::Renderer2D::SetClearColor(glm::vec3(0.1f));

	m_Conf = { false, false, 8000, "127.0.0.1:8000" };
	m_ChatHelper.m_MessageHistory.clear();
	strcpy(m_ChatHelper.m_OwnNick, "User");
	char c[2] = { (char)Eis::Random::UInt('0', '9'), '\0'};
	strcat(m_ChatHelper.m_OwnNick, c);
	c[0] = (char)Eis::Random::UInt('0', '9');
	strcat(m_ChatHelper.m_OwnNick, c);
	c[0] = (char)Eis::Random::UInt('0', '9');
	strcat(m_ChatHelper.m_OwnNick, c);
}

void ChatDemo::Detach()
{
	if (m_Server)
	{
		m_Server->Stop();
		m_Server.reset();
	}
	else if (m_Client)
	{
		m_Client->Disconnect();
		m_Client.reset();
	}
}


void ChatDemo::Render()
{
	Eis::RenderCommands::Clear();
}

void ChatDemo::ImGuiRender()
{
	ImGuiMenu(Eis);

	// Init
	if (!m_Server && !m_Client)
	{
		if (!ImGui::Begin("Init"))
		{
			ImGui::End();
			return;
		}

		// Select program type
		if (!(m_Conf.isServer || m_Conf.isClient))
		{
			ImGui::Text("Select the type of program to run:");
			ImGui::NewLine();
			m_Conf.isServer = ImGui::Button("Server");
			m_Conf.isClient = ImGui::Button("Client") && !m_Conf.isServer;
			
			ImGui::End();
			return;
		}

		// Configure server
		else if (m_Conf.isServer)
		{
			ImGui::Text("Enter the port number to use:");
			ImGui::InputInt("Port", (int*) &m_Conf.port, 0, 0);
			if (ImGui::Button("Run"))
			{
				m_ChatHelper.m_MessageHistory.push_back(fmt::format("Server listening on port {}", m_Conf.port));
				strcpy(m_ChatHelper.m_OwnNick, "Server");
				m_Server = Eis::CreateScope<Eis::Server>(m_Conf.port);
				m_Server->SetClientConnectedCallback([this](const Eis::ClientInfo& info) {
					ChatHelper::Get().m_MessageHistory.push_back(fmt::format("{} connected.", info.ConnectionDescription));
					m_Server->SendStringToAllClients(fmt::format("{} connected.", info.ConnectionDescription), info.Id);
					m_Server->SendStringToClient(info.Id, "Welcome!");
				});
				m_Server->SetClientDisconnectedCallback([this](const Eis::ClientInfo& info) {
					ChatHelper::Get().m_MessageHistory.push_back(fmt::format("{} disconnected.", info.ConnectionDescription));
					m_Server->SendStringToAllClients(fmt::format("{} disconnected.", info.ConnectionDescription), info.Id);
				});
				m_Server->SetDataReceivedCallback([this](const Eis::ClientInfo& info, const Eis::Buffer& buf) {
					m_Server->SendBufferToAllClients(buf, info.Id);
					ChatHelper::Get().m_MessageHistory.push_back(buf.As<const char>());
				});
				m_Server->Start();
			}
		}

		// Configure client
		else if (m_Conf.isClient)
		{
			ImGui::Text("Enter the server address and port:");
			ImGui::InputText("IP", m_Conf.ip, sizeof(m_Conf.ip));
			ImGui::InputText("Nick", m_ChatHelper.m_OwnNick, sizeof(m_ChatHelper.m_OwnNick) - 1);
			if (ImGui::Button("Connect"))
			{
				m_Client = Eis::CreateScope<Eis::Client>();
				m_Client->SetServerConnectedCallback([] {
					ChatHelper::Get().m_MessageHistory.push_back("Connected to server.");
				});
				m_Client->SetServerDisconnectedCallback([] {
					ChatHelper::Get().m_MessageHistory.push_back("Disconnected from server.");
				});
				m_Client->SetDataReceivedCallback([](const Eis::Buffer& buf) {
					ChatHelper::Get().m_MessageHistory.push_back(buf.As<const char>());
				});
				m_Client->ConnectToServer(m_Conf.ip);
			}
		}
		
		if (ImGui::Button("Back"))
			m_Conf.isServer = m_Conf.isClient = false;

		ImGui::End();
	}
	// Gui interface
	else // Running
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(m_Name.c_str()))
		{
			ImGui::End();
			return;
		}

		// Reserve enough left-over height for 1 separator + 1 input text
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			for (const auto& item : m_ChatHelper.m_MessageHistory)
				ImGui::TextUnformatted((item + "\0\n").c_str());

			// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
			// Using a scrollbar or mouse-wheel will take away from the bottom edge.
			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::PopStyleVar();
		}
		ImGui::EndChild();

		ImGui::Separator();

		// Command-line
		bool regainFocus = false;
		if (ImGui::InputText("##", m_MessageBuf, sizeof(m_MessageBuf) - 1, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// Process known commands
			if (m_Server)
			{
				if (strcmp(m_MessageBuf, "/help") == 0)
				{
					m_ChatHelper.m_MessageHistory
						.push_back("/help\nExample chat application - Vlad-07\n\n/help - Display this menu\n/stop - Stop the server\n");
					goto _DONE;
				}
				else if (strcmp(m_MessageBuf, "/stop") == 0)
				{
					m_Server->Stop();
					m_Server.reset();
					m_ChatHelper.m_MessageHistory.clear();
					goto _DONE;
				}
			}
			else // Client
			{
				if (strcmp(m_MessageBuf, "/help") == 0)
				{
					m_ChatHelper.m_MessageHistory
						.push_back("/help\nExample chat applivation - Vlad-07\n\n/help - Display this menu\n/nick NICK - Set your nickname\n/disconnect - Disconnect form server\n");
					goto _DONE;
				}
				else if (strncmp(m_MessageBuf, "/nick", 5) == 0)
				{
					if (strlen(m_MessageBuf) <= 5)
					{
						m_ChatHelper.m_MessageHistory.push_back("/nick NICK");
						goto _DONE;
					}
					else
					{
						strcpy(m_MessageBuf, m_MessageBuf + 6);

						if (strcmp(m_MessageBuf, "Server") == 0)
						{
							m_ChatHelper.m_MessageHistory.push_back("Illegal nickname!");
							goto _DONE;
						}

						m_Client->SendString(fmt::format("{} changed their name to {}", m_ChatHelper.m_OwnNick, m_MessageBuf));
						strcpy(m_ChatHelper.m_OwnNick, m_MessageBuf);
						m_ChatHelper.m_MessageHistory.push_back(fmt::format("Changed your nickname to {}", m_ChatHelper.m_OwnNick));

						goto _DONE;
					}
				}
				else if (strcmp(m_MessageBuf, "/disconnect") == 0)
				{
					m_Client->Disconnect();
					m_Client.reset();
					m_ChatHelper.m_MessageHistory.clear();
					goto _DONE;
				}
			}

			// Regular message
			{
				strcpy(m_MessageBuf, fmt::format("{}: {}", m_ChatHelper.m_OwnNick, m_MessageBuf).c_str());

				m_ChatHelper.m_MessageHistory.push_back(m_MessageBuf);

				if (m_Server) m_Server->SendStringToAllClients(m_MessageBuf);
				else		  m_Client->SendString(m_MessageBuf);
			}

			_DONE:

			m_MessageBuf[0] = '\0';
			regainFocus = true;
		}

		if (regainFocus)
			ImGui::SetKeyboardFocusHere(-1);

		ImGui::End();
	}
}


Eis::Layer::Factory ChatDemo::GetFactory()
{
	return [](const std::string& name) -> Eis::Scope<Layer> { return Eis::CreateScope<ChatDemo>(name); };
}

#endif