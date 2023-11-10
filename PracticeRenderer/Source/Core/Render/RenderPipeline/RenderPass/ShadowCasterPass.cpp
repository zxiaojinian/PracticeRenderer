#include "pch.h"
#include "ShadowCasterPass.h"

#include "Core/Asset/Resources.h"

namespace PR
{
	ShadowCasterPass::ShadowCasterPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		std::shared_ptr<Shader> shadowCaster = Resources::Get().LoadShader("Assets/Shader/ShadowCaster.shader");
		//Temp
		shadowCaster->GetRenderStateBlock().depthState.compareFunction = CompareFunction::LessEqual;
		shadowCaster->GetRenderStateBlock().depthState.writeEnabled = true;
		shadowCaster->GetRenderStateBlock().cullMode = CullMode::Back;
		shadowCaster->GetRenderStateBlock().blendState.WriteMask = ColorWriteMask::None;

		m_ShadowCasterMat = std::make_shared<Material>("ShadowCasterMat");
		m_ShadowCasterMat->SetShader(shadowCaster);

		RenderTextureSpecification specification = { 2048, 2048, TextureFormat::D32_SFloat_S8_UInt, TextureWrapMode::Clamp, TextureFilterMode::Nearest, false };
		m_Shadowmap = RenderTexture::Create("Shadowmap", specification);
		Shader::SetTexture("MainLightShadowmap", m_Shadowmap.get());
	}

	void ShadowCasterPass::Configure()
	{
		ConfigureTarget(nullptr, m_Shadowmap);
	}

	void ShadowCasterPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		graphicsContext.SetViewProjectionMatrices(m_ViewMatrix, m_ProjectionMatrix);
		DrawingSettings drawingSettings;
		drawingSettings.overrideMaterial = m_ShadowCasterMat.get();
		FilteringSettings filteringSettings;
		graphicsContext.DrawRenderers(renderingData.cullResults, drawingSettings, filteringSettings);

		Shader::SetMat4("Matrix_MainLightShadow", m_ProjectionMatrix * m_ViewMatrix);
	}

	bool ShadowCasterPass::Setup(const RenderingData& renderingData)
	{
		uint32_t mainLightIndex = renderingData.mainLightIndex;
		if (mainLightIndex < 0)
			return false;

		auto light = renderingData.cullResults.VisibleLights[mainLightIndex];
		if (!light)
			return false;

		m_ViewMatrix = light->GetTransform().GetWorldToLocalMatrix();
		m_ProjectionMatrix = glm::orthoRH(-50.0f, 20.0f, -20.0f, 20.0f, 0.3f, 500.f);
	}
}
