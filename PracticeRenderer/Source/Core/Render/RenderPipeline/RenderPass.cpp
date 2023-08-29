#include "pch.h"
#include "RenderPass.h"

namespace PR
{
	RenderPass::RenderPass(RenderPassEvent renderPassEvent)
	{
		this->renderPassEvent = renderPassEvent;
	}

	void RenderPass::ConfigureTarget(std::shared_ptr<RenderTexture>& colorAttachment, std::shared_ptr<RenderTexture>& depthAttachment)
	{
		OverrideCameraTarget = true;

		m_ColorAttachment = colorAttachment;
		m_DepthAttachment = depthAttachment;
	}
}
