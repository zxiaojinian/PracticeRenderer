#include "pch.h"
#include "ForwardRenderer.h"

#include "Core/Render/RenderData/LightData.h"

namespace PR
{
	ForwardRenderer::ForwardRenderer()
	{
		RenderTextureSpecification colorSpecification = { 1920, 1080, 1, TextureFormat::R11G11B10_UFloatPack32, TextureWrapMode::Clamp, TextureFilterMode::Bilinear, false};
		m_ColorRenderTexture = RenderTexture::Create("ColorRenderTexture", colorSpecification);
		RenderTextureSpecification depthSpecification = { 1920, 1080, 1, TextureFormat::D32_SFloat_S8_UInt, TextureWrapMode::Clamp, TextureFilterMode::Nearest, false };
		m_DepthRenderTexture = RenderTexture::Create("DepthRenderTexture", depthSpecification);

		m_ShadowCasterPass = std::make_shared<ShadowCasterPass>(RenderPassEvent::BeforeRenderingShadows);
		m_DepthOnlyPass = std::make_shared<DepthOnlyPass>(RenderPassEvent::BeforeRenderingPrepasses);
		m_TiledBaseLightingPass = std::make_shared<TiledBaseLightingPass>(RenderPassEvent::AfterRenderingPrePasses);
		m_DrawObjectsPass = std::make_shared<DrawObjectsPass>(RenderPassEvent::BeforeRenderingOpaques);
		m_DrawSkyboxPass = std::make_shared<DrawSkyboxPass>(RenderPassEvent::BeforeRenderingSkybox);
		m_TiledBaseLightingDebugPass = std::make_shared<TiledBaseLightingDebugPass>(RenderPassEvent::AfterRenderingPostProcessing);
		m_PostProcessPass = std::make_shared<PostProcessPass>(RenderPassEvent::BeforeRenderingPostProcessing);
	}

	void ForwardRenderer::Setup(RenderingData& renderingData)
	{
		ConfigureCameraTarget(m_ColorRenderTexture, m_DepthRenderTexture);

		//Temp
		bool tiledBaseLightingDebug = false;

		if (m_ShadowCasterPass->Setup(renderingData))
			EnqueuePass(m_ShadowCasterPass);

		EnqueuePass(m_DepthOnlyPass);

		m_TiledBaseLightingPass->Setup(tiledBaseLightingDebug);
		EnqueuePass(m_TiledBaseLightingPass);

		EnqueuePass(m_DrawObjectsPass);
		EnqueuePass(m_DrawSkyboxPass);

		if(tiledBaseLightingDebug)
			EnqueuePass(m_TiledBaseLightingDebugPass);

		m_PostProcessPass->Setup(m_ColorRenderTexture);
		EnqueuePass(m_PostProcessPass);
	}

	void ForwardRenderer::SetupLights(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		auto& visibleLights = renderingData.cullResults.VisibleLights;

		int32_t mainLightIndex = renderingData.mainLightIndex;
		auto stride = static_cast<uint32_t>(sizeof(LightData));
		std::vector<LightData> lightsData;
		Light* mainLight = mainLightIndex >= 0 ? visibleLights[mainLightIndex] : nullptr;
		if (mainLight)
		{
			lightsData.push_back(mainLight->GetLightData());
		}
		else
		{
			LightData data{};
			lightsData.push_back(data);
		}

		for (size_t i = 0; i < visibleLights.size(); i++)
		{
			auto light = visibleLights[i];
			if (light && static_cast<int32_t>(i) != mainLightIndex)
				lightsData.push_back(light->GetLightData());
		}

		uint32_t lightNum = static_cast<uint32_t>(lightsData.size());
		if (m_LightDataBuffer == nullptr || m_LightDataBuffer->GetCount() != lightNum)
		{
			m_LightDataBuffer = Buffer::Create(lightNum, sizeof(LightData), BufferType::StorageBuffer, BufferUsage::Dynamic);
			Shader::SetBuffer("LightDataBuffer", m_LightDataBuffer.get());
		}

		m_LightDataBuffer->SetData(lightsData.data(), 0, lightNum, stride);
	}
}
