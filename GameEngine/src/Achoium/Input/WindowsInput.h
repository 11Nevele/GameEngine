#pragma once
#include "acpch.h"
#include "Window\WinWindow.h"
#include "Util\Vector2.h"
namespace ac
{
	class WinWindow; // Forward declaration of WinWindow class
	class WindowsInput
	{
	public:
		bool IsKeyPressed(int keycode, const WinWindow& window);
		bool IsMouseButtonPressed(int button, const WinWindow& window);
		Vector2 GetMousePosition(const WinWindow& window);
	};
}