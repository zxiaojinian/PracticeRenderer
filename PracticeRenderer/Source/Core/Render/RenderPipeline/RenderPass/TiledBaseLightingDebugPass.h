#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class TiledBaseLightingDebugPass : public RenderPass
	{
	public:
		TiledBaseLightingDebugPass(RenderPassEvent renderPassEvent);
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

	private:
		std::shared_ptr<Material> m_DebugMat = nullptr;
	};
}
