#pragma once
#include "acpch.h"
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
	};

	/**
	 * @brief Event data for mouse scroll wheel input.
	 * 
	 * Contains the scroll offsets in both horizontal and vertical directions.
	 */
	struct MouseScrolledEvent
	{
		float xOffset, yOffset; ///< Scroll offsets for horizontal (x) and vertical (y) scrolling
	};

	/**
	 * @brief Event data for when a mouse button is pressed.
	 * 
	 * Contains information about which mouse button was pressed.
	 */
	struct MouseButtonPressedEvent
	{
		int button; ///< The code identifying which mouse button was pressed
	};

	/**
	 * @brief Event data for when a mouse button is released.
	 * 
	 * Contains information about which mouse button was released.
	 */
	struct MouseButtonReleasedEvent
	{
		int button; ///< The code identifying which mouse button was released
	};

}