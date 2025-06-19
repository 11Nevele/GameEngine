#pragma once
#include "acpch.h"
#include "AllowToken.h"
#include "Window/WinWindow.h"
namespace ac
{
	/**
	 * @brief Event data for mouse movement.
	 * 
	 * Contains the current position of the mouse cursor in screen coordinates.
	 */
	struct MouseMovedEvent
	{
		float x, y; ///< Current mouse cursor position (x, y) in screen coordinates
		
		MouseMovedEvent() = default;
		MouseMovedEvent(float x, float y) : x(x), y(y) {}
	};

	ALLOWTOKEN(MouseMovedEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for mouse scroll wheel input.
	 * 
	 * Contains the scroll offsets in both horizontal and vertical directions.
	 */
	struct MouseScrolledEvent
	{
		float xOffset, yOffset; ///< Scroll offsets for horizontal (x) and vertical (y) scrolling
		
		MouseScrolledEvent() = default;
		MouseScrolledEvent(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}
	};

	ALLOWTOKEN(MouseScrolledEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for when a mouse button is pressed.
	 * 
	 * Contains information about which mouse button was pressed.
	 */
	struct MouseButtonPressedEvent
	{
		int button; ///< The code identifying which mouse button was pressed
		
		MouseButtonPressedEvent() = default;
		MouseButtonPressedEvent(int button) : button(button) {}
	};

	ALLOWTOKEN(MouseButtonPressedEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for when a mouse button is released.
	 * 
	 * Contains information about which mouse button was released.
	 */
	struct MouseButtonReleasedEvent
	{
		int button; ///< The code identifying which mouse button was released
		
		MouseButtonReleasedEvent() = default;
		MouseButtonReleasedEvent(int button) : button(button) {}
	};

	ALLOWTOKEN(MouseButtonReleasedEvent, friend class Window; friend class WinWindow;)
}