#include "acpch.h"

#include "WinWindow.h"
#include "Window.h"
#include "Debug.h"
#include "Event\KeyEvent.h"
#include "Event\MouseEvent.h"
#include "Event\WindowEvent.h"

namespace ac
{
	static bool GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		ACERR("GLFW Error: " << description);
	}
	WinWindow::WinWindow(const WindowProperties& properties, EventManager& eventManager): 
		mData({ properties.name, properties.width, properties.height, true, eventManager })
	{
		Init();
		eventManager.RegisterEvent<KeyPressedEvent>()
			.RegisterEvent<KeyReleasedEvent>()
			.RegisterEvent<KeyTypedEvent>()
			.RegisterEvent<MouseButtonPressedEvent>()
			.RegisterEvent<MouseButtonReleasedEvent>()
			.RegisterEvent<MouseMovedEvent>()
			.RegisterEvent<MouseScrolledEvent>()
			.RegisterEvent<WindowCloseEvent>()
			.RegisterEvent<WindowResizeEvent>()
			.RegisterEvent<AppTickEvent>()
			.RegisterEvent<AppUpdateEvent>()
			.RegisterEvent<AppRenderEvent>();

	}
	WinWindow::~WinWindow()
	{
		Shutdown();
	}
	void WinWindow::Init()
	{

		if (!GLFWInitialized)
		{
			int success = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
			if (!success)
			{
				ACERR("Could not initialize GLFW!");
				return;
			}
			GLFWInitialized = true;
		}
		mWindow = glfwCreateWindow(mData.width, mData.height, mData.title.c_str(), nullptr, nullptr);
		if (!mWindow)
		{
			ACERR("Could not create window!");
			return;
		}
		
		mContext = new OpenGLContext(mWindow);
		mContext->Init();

		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);
		
		// Set callbacks using the static member functions
		glfwSetWindowSizeCallback(mWindow, WindowSizeCallback);
		glfwSetWindowCloseCallback(mWindow, WindowCloseCallback);
		glfwSetKeyCallback(mWindow, KeyCallback);
		glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
		glfwSetScrollCallback(mWindow, ScrollCallback);
		glfwSetCursorPosCallback(mWindow, CursorPositionCallback);
	}

	void WinWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;
		WindowResizeEvent event(width, height);
		data.mEventManager.Invoke(event, AllowToken<WindowResizeEvent>());
	}

	void WinWindow::WindowCloseCallback(GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.mEventManager.Invoke(event, AllowToken< WindowCloseEvent>());
	}

	void WinWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key, 0);
			data.mEventManager.Invoke(event, AllowToken<KeyPressedEvent>());
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(key);
			data.mEventManager.Invoke(event, AllowToken< KeyReleasedEvent>());
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			data.mEventManager.Invoke(event,AllowToken< KeyPressedEvent>());
			break;
		}
		}
	}

	void WinWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			data.mEventManager.Invoke(event, AllowToken< MouseButtonPressedEvent>());
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			data.mEventManager.Invoke(event, AllowToken< MouseButtonReleasedEvent>());
			break;
		}
		}
	}

	void WinWindow::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.mEventManager.Invoke(event, AllowToken<MouseScrolledEvent>());
	}

	void WinWindow::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		MouseMovedEvent event = { (float)xPos, (float)yPos };
		data.mEventManager.Invoke(event, AllowToken<MouseMovedEvent>());
	}

	void WinWindow::Shutdown()
	{
		glfwDestroyWindow(mWindow);
		//delete mWindow;
		delete mContext;
	}
	void WinWindow::OnUpdate()
	{
		glfwPollEvents();
		mContext->SwapBuffers();
	}
	void WinWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		mData.VSync = enabled;
	}
	bool WinWindow::IsVSync() const
	{
		return mData.VSync;
	}
}