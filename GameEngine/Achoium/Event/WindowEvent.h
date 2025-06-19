#pragma once
#include "AllowToken.h"
#include "Window/WinWindow.h"
namespace ac
{
	/**
	 * @brief Event data for window close action.
	 * 
	 * Triggered when a window close is requested, such as when the user
	 * clicks the close button or alt-F4 is pressed.
	 */
	struct WindowCloseEvent
	{
		WindowCloseEvent() = default;
	};

	ALLOWTOKEN(WindowCloseEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for window resize actions.
	 * 
	 * Contains the new dimensions of the window after resizing.
	 */
	struct WindowResizeEvent
	{
		unsigned int width, height; ///< The new width and height of the window in pixels
		
		WindowResizeEvent() = default;
		WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}
	};

	ALLOWTOKEN(WindowResizeEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for application tick.
	 * 
	 * Triggered on each tick of the application's main loop,
	 * useful for time-based operations.
	 */
	struct AppTickEvent
	{
		AppTickEvent() = default;
	};

	ALLOWTOKEN(AppTickEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for application update.
	 * 
	 * Triggered during the update phase of the application loop.
	 * Used for game logic, physics, etc.
	 */
	struct AppUpdateEvent
	{
		AppUpdateEvent() = default;
	};

	ALLOWTOKEN(AppUpdateEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for application render.
	 * 
	 * Triggered during the rendering phase of the application loop.
	 * Used to signal when rendering should occur.
	 */
	struct AppRenderEvent
	{
		AppRenderEvent() = default;
	};

	ALLOWTOKEN(AppRenderEvent, friend class Window; friend class WinWindow;)
}