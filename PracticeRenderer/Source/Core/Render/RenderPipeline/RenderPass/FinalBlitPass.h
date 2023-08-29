#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class FinalBlitPass : public RenderPass
	{
	public:
		FinalBlitPass(RenderPassEvent renderPassEvent, std::shared_ptr<Material>& blitMaterial);
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

		void Setup(std::shared_ptr<RenderTexture>& source);

	private:
		std::shared_ptr<Material> m_BlitMaterial;
		std::shared_ptr<RenderTexture> m_Source;
	};
}


