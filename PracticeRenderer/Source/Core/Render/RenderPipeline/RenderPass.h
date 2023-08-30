#pragma once

#include "Core/Render/RenderPipeline/RenderPassEvent.h"
#include "Core/Render/RenderData/RenderingData.h"
#include "Core/Render/GraphicsContext.h"
#include "Core/Render/RenderTexture.h"

namespace PR
{
	class RenderPass
	{
	public:
		RenderPass(RenderPassEvent renderPassEvent);

		std::shared_ptr<RenderTexture>& GetColorAttachment() { return m_ColorAttachment; }
		std::shared_ptr<RenderTexture>& GetDepthAttachment() { return m_DepthAttachment; }

		//This method is called by the renderer before rendering a camera
		virtual void OnCameraSetup() {};
		//This method is called by the renderer before executing the render pass.
		virtual void Configure() {};
		//Called upon finish rendering a camera.
		virtual void OnCameraCleanup() {};
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) = 0;

		void ConfigureTarget(std::shared_ptr<RenderTexture>& colorAttachment, std::shared_ptr<RenderTexture>& depthAttachment);

	public:
		RenderPassEvent renderPassEvent;
		bool OverrideCameraTarget = false;
	private:
		std::shared_ptr<RenderTexture> m_ColorAttachment = nullptr;
		std::shared_ptr<RenderTexture> m_DepthAttachment = nullptr;
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


