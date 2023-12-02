#pragma once

#include "Core/Render/RenderPipeline/Renderer.h"
#include "Core/Render/RenderPipeline/RenderPass/ShadowCasterPass.h"
#include "Core/Render/RenderPipeline/RenderPass/DepthOnlyPass.h"
#include "Core/Render/RenderPipeline/RenderPass/TiledBaseLightingPass.h"
#include "Core/Render/RenderPipeline/RenderPass/DrawObjectsPass.h"
#include "Core/Render/RenderPipeline/RenderPass/DrawSkyboxPass.h"
#include "Core/Render/RenderPipeline/RenderPass/TiledBaseLightingDebugPass.h"
#include "Core/Render/RenderPipeline/RenderPass/PostProcessPass.h"

#include "Core/Render/Asset/Material.h"

namespace PR
{
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer();

		virtual void Setup(RenderingData& renderingData) override;
		virtual void SetupLights(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;
		//virtual void FinishRendering() override;
	private:
		std::shared_ptr<ShadowCasterPass> m_ShadowCasterPass;
		std::shared_ptr<DepthOnlyPass> m_DepthOnlyPass;
		std::shared_ptr<TiledBaseLightingPass> m_TiledBaseLightingPass;
		std::shared_ptr<DrawObjectsPass> m_DrawObjectsPass;
		std::shared_ptr<DrawSkyboxPass> m_DrawSkyboxPass;
		std::shared_ptr<TiledBaseLightingDebugPass> m_TiledBaseLightingDebugPass;
		std::shared_ptr<PostProcessPass> m_PostProcessPass;

		std::shared_ptr<RenderTexture> m_ColorRenderTexture;
		std::shared_ptr<RenderTexture> m_DepthRenderTexture;

		std::shared_ptr<Buffer> m_LightDataBuffer = nullptr;

	};
}

