#pragma once

#include "Core/Common.h"
#include "Core/Render/Framebuffer.h"

namespace PR
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer();
		virtual ~OpenGLFramebuffer();

		virtual void AttachColorTexture(RenderTexture* colorRT, uint32_t level = 0, uint32_t slice = 0) override;
		virtual void AttachColorTexture(std::vector<RenderTexture>& colorRTs) override;
		virtual void AttachDepthTexture(RenderTexture* depthRT, uint32_t level = 0, uint32_t slice = 0) override;
		virtual void Bind() override;
		virtual void Unbind() override;
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_RTNum = 0;
	};
}

