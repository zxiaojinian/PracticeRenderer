#include "pch.h"
#include "Application.h"

#include "Core/Common.h"
#include "Core/Log/Log.h"
#include "Core/Event/EventData/ApplicationEvent.h"

//#include "Core/Scene/GameObject.h"
//#include "Core/Scene/Components/MeshRenderer.h"
//#include "Core/Asset/ShaderLibrary.h"
#include "Core/Asset/ModelLoder.h"
#include "Core/Scene/Components/Transform.h"

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
		m_GraphicsContext->Init();
		m_RenderPipeline = std::make_unique<RenderPipeline>();

		m_SceneManager.LoadScene("");
		auto cameraGo = new GameObject("MainCamera");
		cameraGo->AddComponent<Camera>();
		cameraGo->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 5.0f, 10.0f));

		//GameObject* go = new GameObject("Test GO");
		//go->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//auto& meshRender = go->AddComponent<MeshRenderer>();
		//std::vector<Vertex> v;
		//v.push_back({ {-0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} });
		//v.push_back({ {0.0f, 0.75f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.5f,0.5f} });
		//v.push_back({ {0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} });
		//uint32_t index[3] = { 0, 1, 2 };
		//std::vector<uint32_t> i(index, index + 5);
		//std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Test Mesh", v, i);
		//meshRender.SetMesh(mesh);
		//std::shared_ptr<Shader> shader = Shader::Create("Assets/FlatColor.glsl");
		//std::shared_ptr<Material> mat = std::make_shared<Material>("Test mat");
		//mat->SetShader(shader);
		///*mat->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });*/
		//Shader::SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
		//meshRender.AddMaterial(mat);

		ModelLoder modelLoder;
		modelLoder.LoadModel("Assets/Model/nanosuit/nanosuit.obj");

		OnInit();
	}

	void Application::OnUpdateInternal()
	{
		double time = m_Window->GetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_Window->OnUpdate();
		m_SceneManager.GetCurrentScene()->OnUpdate();

		OnUpdate(timestep);
	}

	void Application::OnRenderInternal()
	{
		m_RenderPipeline->Render(*m_GraphicsContext);
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
		//m_GraphicsContext->OnWindowResize(event.GetWidth(), event.GetHeight());

		PR_LOG_INFO("Window Resize, {0} - {1}", event.GetWidth(), event.GetHeight());
	}
}

