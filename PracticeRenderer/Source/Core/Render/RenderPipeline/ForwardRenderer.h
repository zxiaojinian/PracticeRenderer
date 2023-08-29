#pragma once

#include "Core/Render/RenderPipeline/Renderer.h"
#include "Core/Render/RenderPipeline/RenderPass/DrawObjectsPass.h"
#include "Core/Render/RenderPipeline/RenderPass/FinalBlitPass.h"
#include "Core/Render/Asset/Material.h"


namespace PR
{
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer();

		virtual void Setup(RenderingData& renderingData) override;

	private:
		std::shared_ptr<DrawObjectsPass> m_DrawObjectsPass;
		std::shared_ptr<FinalBlitPass> m_FinalBlitPass;

		std::shared_ptr<RenderTexture> m_ColorRenderTexture;
		std::shared_ptr<RenderTexture> m_DepthRenderTexture;

		std::shared_ptr<Material> m_BlitMaterial;
	};
}

