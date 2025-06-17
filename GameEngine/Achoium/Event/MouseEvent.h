#pragma once
#include "acpch.h"
namespace ac
{
	struct MouseMovedEvent
	{
		float x, y;
	};

	struct MouseScrolledEvent
	{
		float xOffset, yOffset;
	};

	struct MouseButtonPressedEvent
	{
		int button;
	};

	struct MouseButtonReleasedEvent
	{
		int button;
	};

}