#include "pch.h"
#include "Application.h"

#include "Core/Common.h"
#include "Core/Log/Log.h"
#include "Core/Event/EventData/ApplicationEvent.h"

namespace PR
{
	Application* Application::s_Instance = nullptr;
	Application::Application(const std::string& name)
	{
		OnInitInternal();
	}

	Application::~Application()
	{
		OnExitInternal();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->PollEvents();
			if (!m_Minimized)
			{
				OnUpdateInternal();
				OnRenderInternal();
			}
		}
	}

	void Application::OnInitInternal()
	{
		s_Instance = this;

		Log::Init();
		WindowProps windowProps{ "PR", 1920, 1080 };
		m_Window = std::make_unique<Window>(windowProps);
		m_Window->WindowEventDispatchers.AddListener(EventType::WindowResize, BIND_EVENT_FN(Application::OnWindowResize));
		m_Window->WindowEventDispatchers.AddListener(EventType::WindowClose, BIND_EVENT_FN(Application::OnWindowClose));
		m_GraphicsContext = GraphicsContext::Create();

		OnInit();
	}

	void Application::OnUpdateInternal()
	{
		double time = m_Window->GetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_Window->OnUpdate();
		OnUpdate(timestep);
	}

	void Application::OnRenderInternal()
	{
		OnRender();
		m_Window->SwapBuffers();
	}

	void Application::OnExitInternal()
	{
		OnExit();
	}

	void Application::OnWindowClose(Event& e)
	{
		m_Running = false;
		PR_LOG_INFO("Close Window");
	}

	void Application::OnWindowResize(Event& e)
	{
		auto& event = *(WindowResizeEvent*)&e;
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
		}
		else
		{
			m_Minimized = false;
		}

		PR_LOG_INFO("Window Resize, {0} - {1}", event.GetWidth(), event.GetHeight());
	}
}

