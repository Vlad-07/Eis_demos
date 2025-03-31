#pragma once

#include <Eis/Core/Core.h>

#ifdef EIS_NETWORKING_ENABLE

#include <queue>
#include <string>


class ChatHelper
{
public:
	ChatHelper();
	~ChatHelper();

	static ChatHelper& Get() { return *s_Instance; }

public:
	std::vector<std::string> m_MessageHistory;

	char m_OwnNick[64]{};

private:
	static ChatHelper* s_Instance;
};

#endif