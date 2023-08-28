#include "pch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace PR
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer()
	{
		glCreateFramebuffers(1, &m_RendererID);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::AttachColorTexture(RenderTexture& colorRT)
	{
		glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, colorRT.GetRendererID(), 0);
	}

	void OpenGLFramebuffer::AttachColorTexture(std::vector<RenderTexture>& colorRTs)
	{
		PR_ASSERT(colorRTs.size() <= 4, "RT数量大于4");
		uint32_t index = 0;
		for (RenderTexture& rt : colorRTs)
		{
			glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + index, rt.GetRendererID(), 0);
			index++;
		}
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		if (!colorRTs.empty())
		{
			glNamedFramebufferDrawBuffers(m_RendererID, colorRTs.size(), drawBuffers);
		}
		else
		{
			glNamedFramebufferDrawBuffer(m_RendererID, GL_NONE);
		}
	}

	void OpenGLFramebuffer::AttachDepthTexture(RenderTexture& depthRT)
	{
		glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, depthRT.GetRendererID(), 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		//PR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
