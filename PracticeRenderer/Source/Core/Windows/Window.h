#pragma once

#include "Core/Event/Event.h"

#include <GLFW/glfw3.h>

#include <string>
#include <functional>
#include <memory>

namespace PR
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "PracticeRenderer", uint32_t width = 1280, uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props = WindowProps());
		~Window();

		void OnUpdate() {}
		void SwapBuffers();
		void PollEvents() { glfwPollEvents(); }
		uint32_t GetWidth(){ return m_Data.Width; }
		uint32_t GetHeight() { return m_Data.Height; }

		double GetTime() const { return glfwGetTime(); }
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const { return m_Data.VSync; }

		GLFWwindow* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title{};
			uint32_t Width = 1, Height = 1;
			bool VSync = false;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

