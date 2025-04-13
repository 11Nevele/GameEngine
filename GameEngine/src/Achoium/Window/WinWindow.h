#pragma once
#include "acpch.h"
#include "Window.h"


namespace ac
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProperties& properties, EventManager& eventManager);
		virtual ~WinWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return mData.width; }
		unsigned int GetHeight() const override { return mData.height; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		virtual void Init();
		virtual void Shutdown();
	private:
		GLFWwindow* mWindow;
		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSync;
			EventManager& mEventManager;
		};
		WindowData mData;
		
	};
}


