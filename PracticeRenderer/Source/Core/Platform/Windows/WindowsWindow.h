#pragma once

#include "Core/Window.h"

#include <GLFW/glfw3.h>

#include <string>

namespace PR 
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;
		virtual void SwapBuffers() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		virtual double GetTime() const override { return glfwGetTime(); }
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		virtual void* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

