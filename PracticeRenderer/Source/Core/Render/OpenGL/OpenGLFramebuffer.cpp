#include "pch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace PR
{
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
		m_RTNum = 1;
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
		m_RTNum = static_cast<uint32_t>(colorRTs.size());
	}

	void OpenGLFramebuffer::AttachDepthTexture(RenderTexture& depthRT)
	{
		glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, depthRT.GetRendererID(), 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		static GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		if (m_RTNum)
		{
			glNamedFramebufferDrawBuffers(m_RendererID, m_RTNum, drawBuffers);
		}
		else
		{
			glNamedFramebufferDrawBuffer(m_RendererID, GL_NONE);
		}
		PR_ASSERT(glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
