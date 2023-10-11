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
		OnInit();
	}

	Application::~Application()
	{
		OnExit();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->PollEvents();
			if (!m_Minimized)
			{
				OnUpdate();
				OnRender();
			}
		}
	}

	void Application::OnInit()
	{
		s_Instance = this;

		Log::Init();

		WindowProps windowProps{ "PR", 1920, 1080 };
		m_Window = std::make_unique<Window>(windowProps);
		m_Window->WindowEventDispatchers.AddListener(EventType::WindowResize, BIND_EVENT_FN(Application::OnWindowResize));
		m_Window->WindowEventDispatchers.AddListener(EventType::WindowClose, BIND_EVENT_FN(Application::OnWindowClose));

		m_Time.Init(m_Window->GetTime());
		m_GraphicsContext = GraphicsContext::Create();
		m_GraphicsContext->Init();

		m_Resources = std::make_unique<Resources>();
		m_Resources->Init();

		m_SceneManager = std::make_unique<SceneManager>();
		m_RenderPipeline = std::make_unique<RenderPipeline>();
		m_RenderPipeline->Init(*m_GraphicsContext);
	}

	void Application::OnUpdate()
	{
		m_Time.Upadte(m_Window->GetTime());
		m_Window->OnUpdate();
		m_SceneManager->GetCurrentScene()->OnUpdate();

	}

	void Application::OnRender()
	{
		m_RenderPipeline->Render(*m_GraphicsContext);
		m_Window->SwapBuffers();
	}

	void Application::OnExit()
	{

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
		//m_GraphicsContext->OnWindowResize(event.GetWidth(), event.GetHeight());

		PR_LOG_INFO("Window Resize, {0} - {1}", event.GetWidth(), event.GetHeight());
	}
}

