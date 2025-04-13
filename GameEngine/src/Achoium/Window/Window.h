#pragma once
#include "acpch.h"
#include "Event\Event.hpp"
#include "Event\KeyEvent.h"
#include "Event\MouseEvent.h"
#include "Event\WindowEvent.h"
namespace ac
{
	struct WindowProperties
	{
		std::string name;
		unsigned int width;
		unsigned int height;
		WindowProperties(const std::string& name = "Achoium Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: name(name), width(width), height(height) {}
	};
	class Window
	{
	public:

		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		static Window* Create(const WindowProperties& properties, EventManager& eventManager);
	};
}


