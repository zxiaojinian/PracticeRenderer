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

	void RenderPipeline::Render(GraphicsContext& graphicsContext)
	{
		SetupPerFrameShaderConstants();

		Scene* scene = SceneManager::Get().GetCurrentScene();
		if (scene)
		{
			auto& cameras = SortCameras(scene->GetCameras());
			for (auto camera : cameras)
			{
				if (camera)
				{
					CameraData cameraData;
					InitializeCameraData(camera, cameraData);
					RenderSingleCamera(graphicsContext, cameraData);
				}
			}
		}
	}

	void RenderPipeline::RenderSingleCamera(GraphicsContext& graphicsContext, CameraData& cameraData)
	{
		auto renderer = cameraData.render;
		if (renderer)
		{
			renderer->Clear();

			auto cullResults = Cull(*cameraData.camera);
			RenderingData renderingData;
			InitializeRenderingData(renderingData, cameraData);

			renderer->Setup(renderingData);
			renderer->Execute(graphicsContext, renderingData);
		}
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

	Renderer* RenderPipeline::GetRenderer(uint16_t index)
	{
		if (index < 0 || index >= m_Renderers.size())
		{
			index = 0;
		}
		return m_Renderers[index].get();
	}

	void RenderPipeline::InitializeCameraData(Camera* camera, CameraData& cameraData)
	{
		if (camera)
		{
			cameraData.camera = camera;
			cameraData.pixelRect = camera->GetPixelRect();
			cameraData.render = GetRenderer(camera->RendererIndex);
		}
	}

	void RenderPipeline::InitializeRenderingData(RenderingData& renderingData, CameraData& cameraData)
	{
		renderingData.cameraData = cameraData;
	}
}
