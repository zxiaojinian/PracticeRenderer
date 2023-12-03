#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class PostProcessPass : public RenderPass
	{
	public:
		PostProcessPass(RenderPassEvent renderPassEvent);
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

		void Setup(std::shared_ptr<RenderTexture>& source);

	private:
		void DoBloom(GraphicsContext& graphicsContext, const RenderingData& renderingData);
	private:		
		std::shared_ptr<RenderTexture> m_Source;

		//bloom
		const uint32_t k_MaxMipCount = 6;
		uint32_t m_InitWidth = 0;
		uint32_t m_InitHeight = 0;
		std::vector<std::shared_ptr<RenderTexture>> m_BloomMipTexture;
		std::shared_ptr<Material> m_BloomDownsSamplingMat;
		std::shared_ptr<Material> m_BloomUpSamplingMat;
		float bloomStrength = 0.04f;

		//UberPost
		std::shared_ptr<Material> m_UberMat;
	};
}


