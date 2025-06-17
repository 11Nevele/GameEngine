#pragma once
#include "acpch.h"
#include "GraphicContext/GraphicContext.h"
namespace ac
{
	class OpenGLContext : public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* window);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* mWindow;
	};
}