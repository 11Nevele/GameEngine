#pragma once
#include "acpch.h"
#include "Window/WinWindow.h"
#include "AllowToken.h"
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
		
		KeyPressedEvent() = default;
		KeyPressedEvent(int keyCode, int repeatCnt) : keyCode(keyCode), repeatCnt(repeatCnt) {}
	};

	ALLOWTOKEN(KeyPressedEvent, friend class Window; friend class WinWindow;)

	/**
	 * @brief Event data for when a key is released.
	 * 
	 * Contains information about which key was released.
	 */
	struct KeyReleasedEvent
	{
		int keyCode; ///< The code identifying which key was released
		
		KeyReleasedEvent() = default;
		KeyReleasedEvent(int keyCode) : keyCode(keyCode) {}
	};

	ALLOWTOKEN(KeyReleasedEvent, friend class Window; friend class WinWindow;)

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
		
	private:
		friend class WinWindow;
		friend class Window;
		KeyTypedEvent() = default;
		KeyTypedEvent(int keyCode) : keyCode(keyCode) {}
	};
	ALLOWTOKEN(KeyTypedEvent, friend class Window; friend class WinWindow;)
}


