#pragma once

#include "Core/Render/RenderPipeline/RenderPassEvent.h"
#include "Core/Render/RenderData/RenderingData.h"
#include "Core/Render/GraphicsContext.h"

namespace PR
{
	class RenderPass
	{
	public:
		//This method is called by the renderer before rendering a camera
		virtual void OnCameraSetup() {};
		//This method is called by the renderer before executing the render pass.
		virtual void Configure() {};
		//Called upon finish rendering a camera.
		virtual void OnCameraCleanup() {};
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) = 0;

	public:
		RenderPassEvent renderPassEvent;
	};

	inline bool operator<(const RenderPass& lhs, const RenderPass& rhs)
	{
		return lhs.renderPassEvent < rhs.renderPassEvent;
	}

	inline bool operator>(const RenderPass& lhs, const RenderPass& rhs)
	{
		return lhs.renderPassEvent > rhs.renderPassEvent;
	}
}


