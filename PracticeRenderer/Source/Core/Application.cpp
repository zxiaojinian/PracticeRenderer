#include "pch.h"
#include "Application.h"

#include "Core/Common.h"
#include "Core/Log/Log.h"

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
			if (!m_Minimized)
			{
				OnUpdateInternal();
				OnRenderInternal();
			}
			m_Window->PollEvents();
		}
	}

	void Application::OnInitInternal()
	{
		s_Instance = this;

		Log::Init();
		WindowProps windowProps{ "PR", 1950, 1080 };
		m_Window = std::make_unique<Window>(windowProps);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEventInternal));
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

	void Application::OnEventInternal(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		OnEvent(e);
	}

	void Application::OnExitInternal()
	{
		OnExit();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		return false;
	}
}

