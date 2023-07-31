#pragma once

#include "Core/Windows/Window.h"
#include "Core/Render/GraphicsContext.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/Event.h"
#include "Core/Timestep.h"

#include <string>
#include <memory>

namespace PR
{
	class Application
	{
	public:
		Application(const std::string& name = "PracticeRenderer");
		virtual ~Application();

		void Run();

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }
	private:
		void OnInitInternal();
		void OnUpdateInternal();
		void OnRenderInternal();
		void OnEventInternal(Event& e);
		void OnExitInternal();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(Timestep time) {}
		virtual void OnRender() {}
		virtual void OnEvent(Event& e) {}
		virtual void OnExit() {}

	private:
		bool m_Running = true;
		bool m_Minimized = false;
		double m_LastFrameTime = 0;
	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<GraphicsContext> m_GraphicsContext;
	};
}

