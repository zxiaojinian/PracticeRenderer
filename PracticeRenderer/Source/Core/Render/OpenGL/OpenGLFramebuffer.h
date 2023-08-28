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

		virtual void AttachColorTexture(RenderTexture& colorRT) override;
		virtual void AttachColorTexture(std::vector<RenderTexture&>& colorRTs) override;
		virtual void AttachDepthTexture(RenderTexture& depthRT) override;
		virtual void Bind() override;
		virtual void Unbind() override;
	private:
		uint32_t m_RendererID = 0;
	};
}

