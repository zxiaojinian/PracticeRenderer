#pragma once

#include "Core/Windows/Window.h"
#include "Core/Render/GraphicsContext.h"
#include "Core/Event/EventData/BaseEvent.h"
#include "Core/Time/Time.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Render/RenderPipeline/RenderPipeline.h"

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
		void OnInit();
		void OnUpdate();
		void OnRender();
		void OnExit();

		void OnWindowClose(Event& e);
		void OnWindowResize(Event& e);

	private:
		bool m_Running = true;
		bool m_Minimized = false;
	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<GraphicsContext> m_GraphicsContext;
		std::unique_ptr<RenderPipeline> m_RenderPipeline;
		std::unique_ptr<SceneManager> m_SceneManager;
		Time m_Time;
	};
}

