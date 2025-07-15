#pragma once
#include "Core/World.hpp"
namespace ac
{
	class InputManagerSystem
	{
	public:
		static void UpdateInput(World& world);
	};
	class InputManager
	{
	private:
		friend class InputManagerSystem;
		bool mIsKeyPressed[512]{ false };
		bool mIsMouseButtonPressed[8]{ false };
		bool mIsKeyDown[512]{ false };
		bool mIsMouseButtonDown[8]{ false };
		bool mIsKeyUp[512]{ false };
		bool mIsMouseButtonUp[8]{ false };
	public:
		bool IsKeyPressed(int keycode) const
		{
			return mIsKeyPressed[keycode];
		}
		bool IsMouseButtonPressed(int button) const
		{
			return mIsMouseButtonPressed[button];
		}
		bool IsKeyDown(int keycode) const
		{
			return mIsKeyDown[keycode];
		}
		bool IsMouseButtonDown(int button) const
		{
			return mIsMouseButtonDown[button];
		}
		bool IsKeyUp(int keycode) const
		{
			return mIsKeyUp[keycode];
		}
		bool IsMouseButtonUp(int button) const
		{
			return mIsMouseButtonUp[button];
		}

	};
}
