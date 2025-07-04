#include "acpch.h"
#include "WindowsInput.h"

//#include <GLFW/\glfw3.h>

namespace ac
{
	bool WindowsInput::IsKeyPressed(int keycode, const WinWindow& window)
	{
		auto state = glfwGetKey(window.mWindow, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressed(int button, const WinWindow& window)
	{
		auto state = glfwGetMouseButton(window.mWindow, button);
		return state == GLFW_PRESS;
		
	}
	glm::vec2 WindowsInput::GetMousePosition(const WinWindow& window)
	{
		double x, y;
		glfwGetCursorPos(window.mWindow, &x, &y);
		return glm::vec2(x, y);
	}
}