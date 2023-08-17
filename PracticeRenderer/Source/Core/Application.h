#pragma once

#include "Core/Windows/Window.h"
#include "Core/Render/GraphicsContext.h"
#include "Core/Event/EventData/BaseEvent.h"
#include "Core/Timestep.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Render/RenderPipeline.h"

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
		void OnExitInternal();

		void OnWindowClose(Event& e);
		void OnWindowResize(Event& e);
	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(Timestep time) {}
		virtual void OnRender() {}
		virtual void OnExit() {}

	private:
		bool m_Running = true;
		bool m_Minimized = false;
		double m_LastFrameTime = 0;
	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<GraphicsContext> m_GraphicsContext;

		SceneManager m_SceneManager;
		RenderPipeline m_RenderPipeline;
	};
}

