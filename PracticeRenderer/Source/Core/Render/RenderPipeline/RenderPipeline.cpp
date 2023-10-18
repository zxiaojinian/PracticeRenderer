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

	void RenderPipeline::Init(GraphicsContext& graphicsContext)
	{
		EnvironmentLighting(graphicsContext);
	}

	void RenderPipeline::Render(GraphicsContext& graphicsContext)
	{
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
		auto skyCubeMap = Resources::Get().GetCubemap("SkyBox");

		const uint32_t groupSize = 16;

		//IBL Diffuse
		if (m_IrradianceCompute == nullptr)
			m_IrradianceCompute = ComputeShader::Create("Assets/Shader/PBR/GI/IrradianceCompute.compute");

		if (skyCubeMap && m_IrradianceCompute)
		{
			uint32_t irradianceSize = 128;
			if (m_IrradianceCubeMap == nullptr)
			{
				CubemapSpecification specification{};
				specification.Width = irradianceSize;
				specification.FilterMode = TextureFilterMode::Bilinear;
				specification.GenerateMips = false;
				specification.WrapMode = TextureWrapMode::Clamp;
				specification.Format = TextureFormat::R16G16B16A16_SFloat;
				m_IrradianceCubeMap = Cubemap::Create("IrradianceCubeMap", specification);
				Shader::SetCubemap("irradianceMap", m_IrradianceCubeMap.get());
				Resources::Get().AddCubemap(m_IrradianceCubeMap);
			}
			auto dispatchCount = static_cast<uint32_t>(ceil(static_cast<float>(irradianceSize) / groupSize));
			m_IrradianceCompute->SetCubemap("environmentMap", skyCubeMap.get());
			m_IrradianceCompute->SetCubemap("irradianceMap", m_IrradianceCubeMap.get());
			m_IrradianceCompute->SetInt("irradianceMapSize", irradianceSize - 1);
			graphicsContext.DispatchCompute(*m_IrradianceCompute, dispatchCount, dispatchCount, 6);
		}


		//IBL Specular
		if(m_SpecularPrefilter == nullptr)
			m_SpecularPrefilter = ComputeShader::Create("Assets/Shader/PBR/GI/SpecularPrefilter.compute");

		if (skyCubeMap && m_SpecularPrefilter)
		{
			uint32_t specularMapSize = 128;
			if (m_SpecularMap == nullptr)
			{
				CubemapSpecification specification{};
				specification.Width = specularMapSize;
				specification.FilterMode = TextureFilterMode::Trilinear;
				specification.GenerateMips = true;
				specification.WrapMode = TextureWrapMode::Clamp;
				specification.Format = TextureFormat::R16G16B16A16_SFloat;
				m_SpecularMap = Cubemap::Create("SpecularMap", specification);
				Shader::SetCubemap("specularMap", m_SpecularMap.get());
				Resources::Get().AddCubemap(m_SpecularMap);
			}
			auto dispatchCount = static_cast<uint32_t>(ceil(static_cast<float>(specularMapSize) / groupSize));
			m_SpecularPrefilter->SetCubemap("environmentMap", skyCubeMap.get());
			m_SpecularPrefilter->SetInt("environmentMapSize", skyCubeMap->GetWidth());
			uint32_t maxMipLevels = 5;
			for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
			{
				uint32_t mipSize = static_cast<uint32_t>(specularMapSize * std::pow(0.5, mip));
				float roughness = (float)mip / (float)(maxMipLevels - 1);
				m_SpecularPrefilter->SetFloat("roughness", roughness);
				m_SpecularPrefilter->SetCubemap("specularMap", m_SpecularMap.get(), mip);
				m_SpecularPrefilter->SetInt("specularMapSize", mipSize > 1 ? mipSize - 1 : 1);
				graphicsContext.DispatchCompute(*m_SpecularPrefilter, dispatchCount, dispatchCount, 6);
			}
		}
	}
}
