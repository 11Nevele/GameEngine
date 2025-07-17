#include "acpch.h"
#include "WindowsInput.h"

//#include <GLFW/\glfw3.h>

namespace ac
{
	bool WindowsInput::IsKeyPressed(int keycode)
	{
		auto state = glfwGetKey(mWindow->mWindow, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressed(int button)
	{
		auto state = glfwGetMouseButton(mWindow->mWindow, button);
		return state == GLFW_PRESS;
		
	}
	glm::vec2 WindowsInput::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(mWindow->mWindow, &x, &y);
		return glm::vec2(x, y);
	}
}