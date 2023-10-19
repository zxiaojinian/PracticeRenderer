#include "pch.h"
#include "ForwardRenderer.h"

#include "Core/Render/RenderData/LightData.h"
#include "Core/Asset/Resources.h"

namespace PR
{
	ForwardRenderer::ForwardRenderer()
	{
		RenderTextureSpecification colorSpecification = { 1920, 1080, TextureFormat::R11G11B10_UFloatPack32, TextureWrapMode::Clamp, TextureFilterMode::Bilinear, false};
		m_ColorRenderTexture = RenderTexture::Create("ColorRenderTexture", colorSpecification);
		RenderTextureSpecification depthSpecification = { 1920, 1080, TextureFormat::D32_SFloat_S8_UInt, TextureWrapMode::Clamp, TextureFilterMode::Nearest, false };
		m_DepthRenderTexture = RenderTexture::Create("DepthRenderTexture", depthSpecification);

		std::shared_ptr<Shader> shader = Resources::Get().LoadShader("Assets/Shader/Blit.shader");
		//Temp
		shader->GetRenderStateBlock().depthState.compareFunction = CompareFunction::Disabled;
		shader->GetRenderStateBlock().depthState.writeEnabled = false;
		shader->GetRenderStateBlock().cullMode = CullMode::Disabled;
		m_BlitMaterial = std::make_shared<Material>("BlitMaterial");
		m_BlitMaterial->SetShader(shader);

		m_DepthOnlyPass = std::make_shared<DepthOnlyPass>(RenderPassEvent::BeforeRenderingPrepasses);
		m_TiledBaseLightingPass = std::make_shared<TiledBaseLightingPass>(RenderPassEvent::AfterRenderingPrePasses);
		m_DrawObjectsPass = std::make_shared<DrawObjectsPass>(RenderPassEvent::BeforeRenderingOpaques);
		m_DrawSkyboxPass = std::make_shared<DrawSkyboxPass>(RenderPassEvent::BeforeRenderingSkybox);
		m_TiledBaseLightingDebugPass = std::make_shared<TiledBaseLightingDebugPass>(RenderPassEvent::AfterRenderingPostProcessing);
		m_FinalBlitPass = std::make_shared<FinalBlitPass>(RenderPassEvent::AfterRendering, m_BlitMaterial);
	}

	void ForwardRenderer::Setup(RenderingData& renderingData)
	{
		ConfigureCameraTarget(m_ColorRenderTexture, m_DepthRenderTexture);

		//Temp
		bool tiledBaseLightingDebug = true;

		EnqueuePass(m_DepthOnlyPass);

		m_TiledBaseLightingPass->Setup(tiledBaseLightingDebug);
		EnqueuePass(m_TiledBaseLightingPass);

		EnqueuePass(m_DrawObjectsPass);
		EnqueuePass(m_DrawSkyboxPass);

		if(tiledBaseLightingDebug)
			EnqueuePass(m_TiledBaseLightingDebugPass);

		m_FinalBlitPass->Setup(m_ColorRenderTexture);
		EnqueuePass(m_FinalBlitPass);
	}

	void ForwardRenderer::SetupLights(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		auto& visibleLights = renderingData.cullResults.VisibleLights;
		if (m_LightDataBuffer == nullptr || m_LightDataBuffer->GetCount() != visibleLights.size())
		{
			m_LightDataBuffer = Buffer::Create(static_cast<uint32_t>(visibleLights.size()), sizeof(LightData), BufferType::StorageBuffer, BufferUsage::Dynamic);
			Shader::SetBuffer("LightDataBuffer", m_LightDataBuffer.get());
		}

		auto stride = static_cast<uint32_t>(sizeof(LightData));
		uint32_t index = 0;
		for (auto light : visibleLights)
		{
			m_LightDataBuffer->SetData(&light->GetLightData(), index * stride, 1, stride);
			index++;
		}
	}
}
