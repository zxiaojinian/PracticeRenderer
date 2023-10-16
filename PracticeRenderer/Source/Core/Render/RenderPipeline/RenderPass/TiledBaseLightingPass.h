#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class TiledBaseLightingPass : public RenderPass
	{
	public:
		TiledBaseLightingPass(RenderPassEvent renderPassEvent);

		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

	};
}

