#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class FinalBlitPass : public RenderPass
	{
	public:
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;
	};
}


