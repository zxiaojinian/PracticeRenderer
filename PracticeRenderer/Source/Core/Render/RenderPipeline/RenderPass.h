#pragma once

#include "Core/Render/RenderPipeline/RenderPassEvent.h"
#include "Core/Render/RenderData/RenderingData.h"

namespace PR
{
	class RenderPass
	{
	public:
		virtual void OnCameraSetup() {};
		virtual void Configure() {};
		virtual void OnCameraCleanup() {};
		virtual void Execute(const RenderingData& renderingData) = 0;

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


