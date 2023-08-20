#include "pch.h"
#include "RenderPipeline.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Render/RenderPipeline/ForwardRenderer.h"

namespace PR
{
	RenderPipeline::RenderPipeline()
	{
		CreateDefaultRenderer();
	}

	void RenderPipeline::SetRenderPath(RenderPath renderPath)
	{
		m_RenderPath = renderPath;
		CreateDefaultRenderer();
	}

	void RenderPipeline::Render()
	{
		SetupPerFrameShaderConstants();

		Scene* scene = SceneManager::Get().GetCurrentScene();
		auto& cameras = SortCameras(scene->GetCameras());
		for (auto camera : cameras)
		{
			if (camera)
			{
				RenderSingleCamera(*camera);
			}
				
		}
	}

	void RenderPipeline::RenderSingleCamera(Camera& camera)
	{
		auto renderer = GetRenderer(camera.RendererIndex);
		auto cullResults = Cull(camera);
		RenderingData renderingData;
		InitializeRenderingData(renderingData);

		renderer->Setup(renderingData);
		renderer->Execute(renderingData);
	}

	std::shared_ptr<CullingResults> RenderPipeline::Cull(Camera& camera)
	{
		auto cullResults = std::shared_ptr<CullingResults>();
		return cullResults;
	}

	void RenderPipeline::SetupPerFrameShaderConstants()
	{
	}

	std::vector<Camera*> RenderPipeline::SortCameras(const std::vector<Camera*>& cameras)
	{
		return std::vector<Camera*>(cameras);
	}

	void RenderPipeline::CreateDefaultRenderer()
	{
		std::shared_ptr<Renderer> renderer;
		switch (m_RenderPath)
		{
			case PR::RenderPath::Deferred:
			{
				//TEMP
				renderer = std::make_shared<ForwardRenderer>();
				break;
			}
			case PR::RenderPath::Forward:
			default:
			{
				renderer = std::make_shared<ForwardRenderer>();
				break;
			}
		}

		if (m_Renderers.size() > 0)
			m_Renderers.erase(m_Renderers.begin());
		m_Renderers.insert(m_Renderers.begin(), renderer);

	}
	std::shared_ptr<Renderer> RenderPipeline::GetRenderer(uint16_t index)
	{
		if (index < 0 || index >= m_Renderers.size())
		{
			index = 0;
		}
		return m_Renderers[index];
	}

	void RenderPipeline::InitializeRenderingData(RenderingData& renderingData)
	{
	}
}
