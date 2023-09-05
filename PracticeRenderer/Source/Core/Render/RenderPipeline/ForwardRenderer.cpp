#include "pch.h"
#include "ForwardRenderer.h"

#include "Core/Asset/Resources.h"

namespace PR
{
	ForwardRenderer::ForwardRenderer()
	{
		RenderTextureSpecification colorSpecification = { 1920, 1080, RenderTextureFormat::RGBA, TextureWrapMode::Clamp, TextureFilterMode::Bilinear,  true, false };
		m_ColorRenderTexture = RenderTexture::Create("ColorRenderTexture", colorSpecification);
		RenderTextureSpecification depthSpecification = { 1920, 1080, RenderTextureFormat::Depth, TextureWrapMode::Clamp, TextureFilterMode::Nearest, false, false };
		m_DepthRenderTexture = RenderTexture::Create("DepthRenderTexture", depthSpecification);

		std::shared_ptr<Shader> shader = Resources::Get().LoadShader("Assets/Blit.glsl");
		//Temp
		shader->GetRenderStateBlock().depthState.compareFunction = CompareFunction::Disabled;
		shader->GetRenderStateBlock().depthState.writeEnabled = false;
		m_BlitMaterial = std::make_shared<Material>("BlitMaterial");
		m_BlitMaterial->SetShader(shader);

		m_DrawObjectsPass = std::make_shared<DrawObjectsPass>(RenderPassEvent::BeforeRenderingOpaques);
		m_FinalBlitPass = std::make_shared<FinalBlitPass>(RenderPassEvent::AfterRendering, m_BlitMaterial);
	}

	void ForwardRenderer::Setup(RenderingData& renderingData)
	{
		ConfigureCameraTarget(m_ColorRenderTexture, m_DepthRenderTexture);

		EnqueuePass(m_DrawObjectsPass);

		m_FinalBlitPass->Setup(m_ColorRenderTexture);
		EnqueuePass(m_FinalBlitPass);
	}
}
