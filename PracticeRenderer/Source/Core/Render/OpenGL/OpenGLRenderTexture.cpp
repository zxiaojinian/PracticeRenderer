#include "pch.h"
#include "OpenGLRenderTexture.h"

#include "OpenGLTextureUtil.h"
#include "Core/Common.h"

#include <glad/glad.h>

namespace PR
{
	OpenGLRenderTexture::OpenGLRenderTexture(const std::string& name, const RenderTextureSpecification& specification)
		: m_Name(name), m_RenderTextureSpecification(specification)
	{
		Init();
	}

	OpenGLRenderTexture::~OpenGLRenderTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLRenderTexture::Resize(uint32_t width, uint32_t height)
	{
		if (m_RenderTextureSpecification.Width != width || m_RenderTextureSpecification.Height != height)
		{
			m_RenderTextureSpecification.Width = width;
			m_RenderTextureSpecification.Height = height;

			glDeleteTextures(1, &m_RendererID);
			Init();
		}
	}

	void OpenGLRenderTexture::SetFilterMode(TextureFilterMode filterMode)
	{
		m_RenderTextureSpecification.FilterMode = filterMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(m_RenderTextureSpecification.FilterMode, m_RenderTextureSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(m_RenderTextureSpecification.FilterMode));
	}

	void OpenGLRenderTexture::SetWrapMode(TextureWrapMode wrapMode)
	{
		m_RenderTextureSpecification.WrapMode = wrapMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(m_RenderTextureSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(m_RenderTextureSpecification.WrapMode));
	}

	void OpenGLRenderTexture::SetTextureFormat(TextureFormat format)
	{
		if (m_RenderTextureSpecification.Format != format)
		{
			m_RenderTextureSpecification.Format = format;
			glDeleteTextures(1, &m_RendererID);
			Init();
		}
	}

	void OpenGLRenderTexture::SetGenerateMips(bool generateMips)
	{
		if (m_RenderTextureSpecification.GenerateMips != generateMips)
		{
			m_RenderTextureSpecification.GenerateMips = generateMips;
			if (m_RenderTextureSpecification.GenerateMips)
				glGenerateTextureMipmap(m_RendererID);
		}
	}

	void OpenGLRenderTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLRenderTexture::Init()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//glBindTexture(GL_TEXTURE_2D, m_RendererID);
		//glTexImage2D(GL_TEXTURE_2D, 0, GetOpenGLInternalFormat(m_RenderTextureSpecification.Format), m_RenderTextureSpecification.Width, m_RenderTextureSpecification.Height,
		//	0, RTFormatToOpenGLDataFormat(m_RenderTextureSpecification.Format), GL_UNSIGNED_BYTE, nullptr);

		GLsizei mipmapCount = m_RenderTextureSpecification.GenerateMips ? static_cast<GLsizei>(std::floor(std::log2((std::max)(m_RenderTextureSpecification.Width, m_RenderTextureSpecification.Height))) + 1) : 1;
		glTextureStorage2D(m_RendererID, mipmapCount, GetOpenGLInternalFormat(m_RenderTextureSpecification.Format), m_RenderTextureSpecification.Width, m_RenderTextureSpecification.Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(m_RenderTextureSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(m_RenderTextureSpecification.WrapMode));

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(m_RenderTextureSpecification.FilterMode, m_RenderTextureSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(m_RenderTextureSpecification.FilterMode));
	}
}
