#include "acpch.h"
#include "OpenGLContext.h"
#include "Debug.h"

namespace ac
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
	{
		ACASSERT(window, "Window is null!");
		mWindow = window;
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(mWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ACASSERT(status, "Failed to initialize GLAD!");

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindow);
	}
}