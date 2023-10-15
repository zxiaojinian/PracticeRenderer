#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class DepthOnlyPass : public RenderPass
	{
	public:
		DepthOnlyPass(RenderPassEvent renderPassEvent);

		virtual void Configure() override;
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

	private:
		std::shared_ptr<RenderTexture> m_CmeraDepthTexture;

		std::shared_ptr<Material> m_DepthMat;
	};
}


