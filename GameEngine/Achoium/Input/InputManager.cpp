#include <acpch.h>
#include "InputManager.h"
#include "WindowsInput.h"
#include "Keycode.h"

void ac::InputManagerSystem::UpdateInput(World& world)
{
	InputManager& inputManager = world.GetResourse<InputManager>();
	WindowsInput& windowsInput = world.GetResourse<WindowsInput>();
	// Update key states
	for (int i = 0; i < AC_KEY_COUNT; ++i)
	{
		int t = keycodeList[i];
		inputManager.mIsKeyDown[t] = !inputManager.mIsKeyPressed[t] && windowsInput.IsKeyPressed(t);
		inputManager.mIsKeyUp[t] = inputManager.mIsKeyPressed[t] && !windowsInput.IsKeyPressed(t);
		inputManager.mIsKeyPressed[t] = windowsInput.IsKeyPressed(t);
	}
	// Update mouse button states
	for (int i = 0; i < 8; ++i)
	{
		inputManager.mIsMouseButtonDown[i] = !inputManager.mIsMouseButtonPressed[i] && windowsInput.IsMouseButtonPressed(i);
		inputManager.mIsMouseButtonUp[i] = inputManager.mIsMouseButtonPressed[i] && !windowsInput.IsMouseButtonPressed(i);
		inputManager.mIsMouseButtonPressed[i] = windowsInput.IsMouseButtonPressed(i);
	}
}
