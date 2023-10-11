#include "pch.h"
#include "RenderPipeline.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Render/RenderPipeline/ForwardRenderer.h"
#include "Core/Asset/Resources.h"

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
		EnvironmentLighting(graphicsContext);
		SetupPerFrameShaderConstants(graphicsContext);

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
			InitializeRenderingData(renderingData, cameraData, cullResults);

			renderer->Setup(renderingData);
			renderer->Execute(graphicsContext, renderingData);
		}
	}

	CullingResults RenderPipeline::Cull(Camera& camera)
	{
		CullingResults cullResults;
		auto& lights = SceneManager::Get().GetCurrentScene()->GetLights();
		auto& goes = SceneManager::Get().GetCurrentScene()->GetGameObjecs();
		for (auto light : lights)
		{
			//TODO Cull
			cullResults.VisibleLights.push_back(light);
		}

		for (auto go : goes)
		{
			//TODO Cull
			cullResults.VisibleGameobjects.push_back(go);
		}

		return cullResults;
	}

	void RenderPipeline::SetupPerFrameShaderConstants(GraphicsContext& graphicsContext)
	{
		//auto lights = SceneManager::Get().GetCurrentScene()->GetLights();
		//if(lights.size())
		//	Shader::SetFloat4("u_LightPosition", lights[0]->GetLightData().Position);
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

	void RenderPipeline::InitializeRenderingData(RenderingData& renderingData, CameraData& cameraData, CullingResults& cullResult)
	{
		renderingData.cameraData = cameraData;
		renderingData.cullResults = cullResult;
	}

	void RenderPipeline::EnvironmentLighting(GraphicsContext& graphicsContext)
	{
		if (m_IrradianceCompute == nullptr)
			m_IrradianceCompute = ComputeShader::Create("Assets/Shader/PBR/GI/IrradianceCompute.compute");

		if (skyCubeMap == nullptr)
		{
			skyCubeMap = Resources::Get().GetCubemap("SkyBox");
		}

		graphicsContext.DispatchCompute(*m_IrradianceCompute, 1, 1, 1);
	}
}
