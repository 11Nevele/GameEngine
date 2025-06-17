#pragma once
#include "acpch.h"
namespace ac
{
	/**
	 * @brief Event data for when a key is pressed.
	 * 
	 * Contains information about which key was pressed and
	 * how many times it has been repeated if held down.
	 */
	struct KeyPressedEvent
	{
		int keyCode;   ///< The code identifying which key was pressed
		int repeatCnt; ///< How many repeat counts have occurred (0 for first press)
	};

	/**
	 * @brief Event data for when a key is released.
	 * 
	 * Contains information about which key was released.
	 */
	struct KeyReleasedEvent
	{
		int keyCode; ///< The code identifying which key was released
	};

	/**
	 * @brief Event data for when a key is typed (character input).
	 * 
	 * Used for text input events, where the key produces a character.
	 * This differs from KeyPressedEvent as it represents actual character
	 * input rather than raw key presses.
	 */
	struct KeyTypedEvent
	{
		int keyCode; ///< The character code that was typed
	};
}


