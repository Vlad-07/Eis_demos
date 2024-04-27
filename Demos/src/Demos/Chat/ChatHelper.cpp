#include "ChatHelper.h"

#include <Eis/Core/Log.h>


ChatHelper* ChatHelper::s_Instance = nullptr;


ChatHelper::ChatHelper()
{
	EIS_ASSERT(!s_Instance, "Trying to reinstantiate ChatHelper");
	s_Instance = this;
}

ChatHelper::~ChatHelper()
{
	s_Instance = nullptr;
}