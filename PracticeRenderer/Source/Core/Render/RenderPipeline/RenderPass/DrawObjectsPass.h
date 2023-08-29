#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class DrawObjectsPass : public RenderPass
	{
	public:
		DrawObjectsPass(RenderPassEvent renderPassEvent);

		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

	private:
		std::shared_ptr<RenderTexture> m_ColorRenderTexture;
		std::shared_ptr<RenderTexture> m_DepthRenderTexture;
	};
}

