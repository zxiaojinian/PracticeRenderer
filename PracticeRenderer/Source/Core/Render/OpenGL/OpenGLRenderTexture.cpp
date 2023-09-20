#include "pch.h"
#include "OpenGLRenderTexture.h"

#include "OpenGLTextureUtil.h"
#include "Core/Common.h"

#include <glad/glad.h>

namespace PR
{
	uint32_t RTFormatToBPP(RenderTextureFormat format)
	{
		switch (format)
		{
			case PR::RenderTextureFormat::R:
				return 1;
			case PR::RenderTextureFormat::RGB:
				return 3;
			case PR::RenderTextureFormat::RGBA:
				return 4;
			case PR::RenderTextureFormat::Depth:
				return 4;
			default:
				return 4;
		}
	}

	GLenum RTFormatToOpenGLInternalFormat(RenderTextureFormat format)
	{
		switch (format)
		{
			case RenderTextureFormat::Depth: return GL_DEPTH24_STENCIL8;
			case RenderTextureFormat::R: return GL_R8;
			case RenderTextureFormat::RGB: return GL_RGB8;
			case RenderTextureFormat::RGBA: return GL_RGBA8;
			default: return GL_RGBA8;
		}
	}

	GLenum RTFormatToOpenGLDataFormat(RenderTextureFormat format)
	{
		switch (format)
		{
			case RenderTextureFormat::Depth:
				return GL_DEPTH_COMPONENT;
			case RenderTextureFormat::R:
				return GL_R;
			case RenderTextureFormat::RGB:
				return GL_RGB;
			case RenderTextureFormat::RGBA:
				return GL_RGBA;
			default:
				return GL_RGBA;
		}
	}

	OpenGLRenderTexture::OpenGLRenderTexture(const std::string& name, const RenderTextureSpecification& specification)
		: m_Name(name), m_RenderTextureSpecification(specification)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//glBindTexture(GL_TEXTURE_2D, m_RendererID);
		//glTexImage2D(GL_TEXTURE_2D, 0, RTFormatToOpenGLInternalFormat(m_RenderTextureSpecification.Format), m_RenderTextureSpecification.Width, m_RenderTextureSpecification.Height,
		//	0, RTFormatToOpenGLDataFormat(m_RenderTextureSpecification.Format), GL_UNSIGNED_BYTE, nullptr);
		glTextureStorage2D(m_RendererID, 1, RTFormatToOpenGLInternalFormat(m_RenderTextureSpecification.Format), m_RenderTextureSpecification.Width, m_RenderTextureSpecification.Height);
		SetWrapMode(m_RenderTextureSpecification.WrapMode);
		SetFilterMode(m_RenderTextureSpecification.FilterMode);
	}

	OpenGLRenderTexture::~OpenGLRenderTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLRenderTexture::Resize(uint32_t width, uint32_t height)
	{
		//m_RenderTextureSpecification.Width = width;
		//m_RenderTextureSpecification.Height = height;

		//glBindTexture(GL_TEXTURE_2D, m_RendererID);
		//glTexImage2D(GL_TEXTURE_2D, 0, RTFormatToOpenGLInternalFormat(m_RenderTextureSpecification.Format), m_RenderTextureSpecification.Width, m_RenderTextureSpecification.Height,
		//	0, RTFormatToOpenGLDataFormat(m_RenderTextureSpecification.Format), GL_UNSIGNED_BYTE, nullptr);

		//if (m_RenderTextureSpecification.GenerateMips)
		//	glGenerateTextureMipmap(m_RendererID);

		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRenderTexture::SetFilterMode(TextureFilterMode filterMode)
	{
		m_RenderTextureSpecification.FilterMode = filterMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGLFilterMode(m_RenderTextureSpecification.FilterMode, m_RenderTextureSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGLFilterMode(m_RenderTextureSpecification.FilterMode));
	}

	void OpenGLRenderTexture::SetWrapMode(TextureWrapMode wrapMode)
	{
		m_RenderTextureSpecification.WrapMode = wrapMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenGLWrapMode(m_RenderTextureSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenGLWrapMode(m_RenderTextureSpecification.WrapMode));
	}

	void OpenGLRenderTexture::SetGenerateMips(bool generateMips)
	{
		//if (m_RenderTextureSpecification.GenerateMips != generateMips)
		//{
		//	m_RenderTextureSpecification.GenerateMips = generateMips;
		//	if (m_RenderTextureSpecification.GenerateMips)
		//		glGenerateTextureMipmap(m_RendererID);
		//}
	}

	void OpenGLRenderTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}
