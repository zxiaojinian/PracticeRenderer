#include "pch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace PR
{
	static const uint32_t s_MaxFramebufferSize = 8192;
	//static const GLenum drawBuffers[][] = {
	//	{}
	//}

	//namespace Utils
	//{

	//	static void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index)
	//	{
	//		bool multisampled = samples > 1;
	//		if (multisampled)
	//		{
	//			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
	//		}
	//		else
	//		{
	//			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//		}

	//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	//	}


	//	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	//	{
	//		bool multisampled = samples > 1;
	//		if (multisampled)
	//		{
	//			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
	//		}
	//		else
	//		{

	//			//glTexImage2D尺寸可变
	//			//glTexStorage2D尺寸不可变，速度更快
	//			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//		}

	//		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	//	}
	//}

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

	void OpenGLFramebuffer::AttachColorTexture(std::vector<RenderTexture&>& colorRTs)
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

	//void OpenGLFramebuffer::Invalidate()
	//{
	//	if (m_RendererID)
	//	{
	//		glDeleteFramebuffers(1, &m_RendererID);
	//		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
	//		glDeleteTextures(1, &m_DepthAttachment);

	//		m_ColorAttachments.clear();
	//		m_DepthAttachment = 0;
	//	}

	//	glCreateFramebuffers(1, &m_RendererID);
	//	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	//	bool multisample = m_Specification.Samples > 1;

	//	if (m_ColorAttachmentSpecifications.size())
	//	{
	//		m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
	//		Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

	//		for (size_t i = 0; i < m_ColorAttachments.size(); i++)
	//		{
	//			Utils::BindTexture(multisample, m_ColorAttachments[i]);
	//			switch (m_ColorAttachmentSpecifications[i].TextureFormat)
	//			{
	//			case FramebufferTextureFormat::RGBA8:
	//				Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, m_Specification.Width, m_Specification.Height, i);
	//				break;
	//			}
	//		}
	//	}

	//	if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
	//	{
	//		Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
	//		Utils::BindTexture(multisample, m_DepthAttachment);
	//		switch (m_DepthAttachmentSpecification.TextureFormat)
	//		{
	//		case FramebufferTextureFormat::DEPTH24STENCIL8:
	//			Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
	//			break;
	//		}
	//	}

	//	if (m_ColorAttachments.size() > 1)
	//	{
	//		PR_ASSERT(m_ColorAttachments.size() <= 4, "RT数量大于4");
	//		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	//		glDrawBuffers(m_ColorAttachments.size(), buffers);
	//	}
	//	else if (m_ColorAttachments.empty())
	//	{
	//		//only depth pass
	//		glDrawBuffer(GL_NONE);
	//	}

	//	PR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//}

	//void OpenGLFramebuffer::Bind()
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	//	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	//}

	//void OpenGLFramebuffer::Unbind()
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//}
}
