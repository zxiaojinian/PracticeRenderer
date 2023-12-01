#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class PostProcessPass : public RenderPass
	{
	public:
		PostProcessPass(RenderPassEvent renderPassEvent, std::shared_ptr<Material>& blitMaterial);
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

		void Setup(std::shared_ptr<RenderTexture>& source);

	private:
		std::shared_ptr<Material> m_BlitMaterial;
		std::shared_ptr<RenderTexture> m_Source;

		//bloom
		const uint32_t k_MaxMipCount = 16;
		uint32_t m_InitWidth = 0;
		uint32_t m_InitHeight = 0;
		std::vector<std::shared_ptr<RenderTexture>> m_BloomMipTexture;
	};
}


