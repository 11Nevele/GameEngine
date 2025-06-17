#pragma once
#include "acpch.h"
#include <glm\glm.hpp>
#include "Window\WinWindow.h"
namespace ac
{
	class WinWindow; // Forward declaration of WinWindow class

	/**
	 * @brief Windows-specific input handling implementation.
	 * 
	 * Provides methods to query keyboard and mouse input states
	 * specific to Windows platform using the GLFW library.
	 */
	class WindowsInput
	{
	public:
		/**
		 * @brief Checks if a specific key is currently pressed.
		 * 
		 * @param keycode The code of the key to check
		 * @param window The window to check input against
		 * @return true If the key is currently pressed
		 * @return false If the key is not currently pressed
		 */
		bool IsKeyPressed(int keycode, const WinWindow& window);

		/**
		 * @brief Checks if a specific mouse button is currently pressed.
		 * 
		 * @param button The code of the mouse button to check
		 * @param window The window to check input against
		 * @return true If the mouse button is currently pressed
		 * @return false If the mouse button is not currently pressed
		 */
		bool IsMouseButtonPressed(int button, const WinWindow& window);

		/**
		 * @brief Gets the current mouse cursor position.
		 * 
		 * @param window The window to check input against
		 * @return glm::vec2 The current mouse position in screen coordinates (x, y)
		 */
		glm::vec2 GetMousePosition(const WinWindow& window);
	};
}