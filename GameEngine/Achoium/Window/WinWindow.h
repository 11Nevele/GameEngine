#pragma once
#include "acpch.h"
#include "Window.h"
#include "Input\WindowsInput.h"
#include "GraphicContext\OpenGLContext.h"
#include "Event\Event.hpp"
namespace ac
{
	/**
	 * @brief Windows-specific implementation of the Window interface.
	 * 
	 * This class provides the Windows platform-specific implementation of the
	 * Window interface using GLFW for window management and event handling.
	 */
	class WinWindow : public Window
	{
	public:
		friend class WindowsInput;
		
		/**
		 * @brief Constructs a window with the specified properties.
		 * 
		 * Creates and initializes a GLFW window with the provided dimensions and title,
		 * and sets up event callbacks through the provided event manager.
		 * 
		 * @param properties Properties defining window size, title, etc.
		 * @param eventManager Reference to the event manager for dispatching window events
		 */
		WinWindow(const WindowProperties& properties, EventManager& eventManager);
		
		/**
		 * @brief Destructor. Cleans up window resources.
		 */
		virtual ~WinWindow();

		/**
		 * @brief Updates the window, processing messages and events.
		 * 
		 * Should be called once per frame to process window events and swap buffers.
		 */
		void OnUpdate() override;

		/**
		 * @brief Gets the current width of the window.
		 * 
		 * @return unsigned int Current window width in pixels
		 */
		unsigned int GetWidth() const override { return mData.width; }
		
		/**
		 * @brief Gets the current height of the window.
		 * 
		 * @return unsigned int Current window height in pixels
		 */
		unsigned int GetHeight() const override { return mData.height; }

		/**
		 * @brief Enables or disables vertical synchronization.
		 * 
		 * @param enabled Whether to enable or disable VSync
		 */
		void SetVSync(bool enabled) override;
		
		/**
		 * @brief Checks if vertical synchronization is enabled.
		 * 
		 * @return true If VSync is enabled
		 * @return false If VSync is disabled
		 */
		bool IsVSync() const override;
		
		/**
		 * @brief Initializes the window and creates the rendering context.
		 * 
		 * Sets up the GLFW window, configures callbacks, and initializes the OpenGL context.
		 */
		virtual void Init();
		
		/**
		 * @brief Shuts down the window and cleans up resources.
		 * 
		 * Destroys the GLFW window and cleans up any associated resources.
		 */
		virtual void Shutdown();
	private:
		GLFWwindow* mWindow;  ///< Handle to the GLFW window
		OpenGLContext* mContext;  ///< OpenGL rendering context for this window
		
		/**
		 * @brief Structure containing window-specific data.
		 * 
		 * Stores information about the window state and configuration.
		 */
		struct WindowData
		{
			std::string title;           ///< Window title
			unsigned int width, height;  ///< Window dimensions
			bool VSync;                  ///< Whether VSync is enabled
			EventManager& mEventManager; ///< Reference to the event manager
		};
		
		WindowData mData;  ///< Window state and configuration data

		// Callback functions for GLFW events
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
		static void WindowCloseCallback(GLFWwindow* window);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	};
}


