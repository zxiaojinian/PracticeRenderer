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

	void OpenGLFramebuffer::AttachColorTexture(RenderTexture* colorRT, uint32_t level, uint32_t slice)
	{
		if (colorRT)
		{
			if (colorRT->GetTextureDimension() == TextureDimension::Tex2DArray)
			{
				glNamedFramebufferTextureLayer(m_RendererID, GL_COLOR_ATTACHMENT0, colorRT->GetRendererID(), level, slice);
			}
			else
			{
				glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, colorRT->GetRendererID(), level);
			}

			m_RTNum = 1;
		}
		else
		{
			glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, 0, 0);
			m_RTNum = 0;
		}
	}

	void OpenGLFramebuffer::AttachColorTexture(std::vector<RenderTexture>& colorRTs)
	{
		PR_ASSERT(colorRTs.size() <= 4, "RT数量大于4");
		for (uint32_t i = 0; i < m_RTNum; ++i)
		{
			glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + i, 0, 0);
		}

		uint32_t index = 0;
		for (RenderTexture& rt : colorRTs)
		{
			glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + index, rt.GetRendererID(), 0);
			index++;
		}
		m_RTNum = static_cast<uint32_t>(colorRTs.size());
	}

	void OpenGLFramebuffer::AttachDepthTexture(RenderTexture* depthRT, uint32_t level, uint32_t slice)
	{
		if (depthRT)
		{
			if (depthRT->GetTextureDimension() == TextureDimension::Tex2DArray)
			{
				glNamedFramebufferTextureLayer(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, depthRT->GetRendererID(), level, slice);
			}
			else
			{
				glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, depthRT->GetRendererID(), level);
			}
		}
		else
			glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, 0, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		if (m_RTNum)
		{
			glNamedFramebufferDrawBuffers(m_RendererID, m_RTNum, drawBuffers);
		}
		else
		{
			glNamedFramebufferDrawBuffer(m_RendererID, GL_NONE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		PR_ASSERT(glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
