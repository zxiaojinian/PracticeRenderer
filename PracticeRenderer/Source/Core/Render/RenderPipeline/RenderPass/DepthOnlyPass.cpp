#include "pch.h"
#include "DepthOnlyPass.h"

#include "Core/Asset/Resources.h"
#include "Core/Render/Asset/ComputeShader.h"

namespace PR
{
	DepthOnlyPass::DepthOnlyPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		std::shared_ptr<Shader> depthOnly = Resources::Get().LoadShader("Assets/Shader/DepthOnly.shader");
		//Temp
		depthOnly->GetRenderStateBlock().depthState.compareFunction = CompareFunction::LessEqual;
		depthOnly->GetRenderStateBlock().depthState.writeEnabled = true;
		depthOnly->GetRenderStateBlock().cullMode = CullMode::Back;
		m_DepthMat = std::make_shared<Material>("DepthMat");
		m_DepthMat->SetShader(depthOnly);

		RenderTextureSpecification depthSpecification = { 1920, 1080, TextureFormat::D32_SFloat_S8_UInt, TextureWrapMode::Clamp, TextureFilterMode::Nearest, false };
		m_CmeraDepthTexture = RenderTexture::Create("CmeraDepthTexture", depthSpecification);
		ComputeShader::SetGlobalTexture("CmeraDepthTexture", m_CmeraDepthTexture.get());
	}

	void DepthOnlyPass::Configure()
	{
		ConfigureTarget(nullptr, m_CmeraDepthTexture);
	}

	void DepthOnlyPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		DrawingSettings drawingSettings;
		drawingSettings.overrideMaterial = m_DepthMat.get();
		FilteringSettings filteringSettings;
		graphicsContext.DrawRenderers(renderingData.cullResults, drawingSettings, filteringSettings);
	}
}
