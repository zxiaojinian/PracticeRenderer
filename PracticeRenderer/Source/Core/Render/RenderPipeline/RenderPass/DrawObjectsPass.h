#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	class DrawObjectsPass : public RenderPass
	{
	public:
		virtual void Execute(const RenderingData& renderingData) override;
	};
}

