#pragma once
#include "acpch.h"
namespace ac
{
	struct KeyPressedEvent
	{
		int keyCode;
		int repeatCnt;
	};

	struct KeyReleasedEvent
	{
		int keyCode;
	};
	struct KeyTypedEvent
	{
		int keyCode;
	};

}


