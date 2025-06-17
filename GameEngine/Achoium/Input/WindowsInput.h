#pragma once
#include "acpch.h"
#include <glm\glm.hpp>
#include "Window\WinWindow.h"
namespace ac
{
	class WinWindow; // Forward declaration of WinWindow class
	class WindowsInput
	{
	public:
		bool IsKeyPressed(int keycode, const WinWindow& window);
		bool IsMouseButtonPressed(int button, const WinWindow& window);
		glm::vec2 GetMousePosition(const WinWindow& window);
	};
}