#include "pch.h"
#include "OpenGLTexture2D.h"

#include "Core/Common.h"
#include "OpenGLTextureUtil.h"

#include <glad/glad.h>

namespace PR
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& name, const Texture2DSpecification& specification)
		: m_Name(name),  m_Texture2DSpecification(specification)
	{
		Init();
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetFilterMode(TextureFilterMode filterMode)
	{
		m_Texture2DSpecification.FilterMode = filterMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(m_Texture2DSpecification.FilterMode, m_Texture2DSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(m_Texture2DSpecification.FilterMode));
	}

	void OpenGLTexture2D::SetWrapMode(TextureWrapMode wrapMode)
	{
		m_Texture2DSpecification.WrapMode = wrapMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(m_Texture2DSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(m_Texture2DSpecification.WrapMode));
	}

	void OpenGLTexture2D::SetTextureFormat(TextureFormat format)
	{
		if (m_Texture2DSpecification.Format != format)
		{
			m_Texture2DSpecification.Format = format;
			glDeleteTextures(1, &m_RendererID);
			Init();
		}
	}

	void OpenGLTexture2D::SetGenerateMips(bool generateMips)
	{
		if (m_Texture2DSpecification.GenerateMips != generateMips)
		{
			m_Texture2DSpecification.GenerateMips = generateMips;
			if (m_Texture2DSpecification.GenerateMips)
				glGenerateTextureMipmap(m_RendererID);
		}
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t x, uint32_t y, uint32_t width, uint32_t height, TexturePixelType dataType)
	{
		PR_ASSERT((width <= m_Texture2DSpecification.Width && height <= m_Texture2DSpecification.Height), "width/height error");

		if (width <= m_Texture2DSpecification.Width && height <= m_Texture2DSpecification.Height)
		{
			glTextureSubImage2D(m_RendererID, 0, x, y, width, height, GetOpenGLPixelFormat(m_Texture2DSpecification.Format), GetOpenGLPixelType(dataType), data);
			if (m_Texture2DSpecification.GenerateMips)
				glGenerateTextureMipmap(m_RendererID);
		}
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::BindImage(uint32_t slot, uint32_t level, TextureAccess access, uint32_t slice) const
	{
		glBindImageTexture(slot, m_RendererID, level, GL_FALSE, 0, GetOpenGLTextureAccess(access), GetOpenGLInternalFormat(m_Texture2DSpecification.Format));
	}

	void OpenGLTexture2D::Init()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		GLsizei mipmapCount = m_Texture2DSpecification.GenerateMips ? static_cast<GLsizei>(std::floor(std::log2((std::max)(m_Texture2DSpecification.Width, m_Texture2DSpecification.Height))) + 1) : 1;
		glTextureStorage2D(m_RendererID, mipmapCount, GetOpenGLInternalFormat(m_Texture2DSpecification.Format), m_Texture2DSpecification.Width, m_Texture2DSpecification.Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(m_Texture2DSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(m_Texture2DSpecification.WrapMode));

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(m_Texture2DSpecification.FilterMode, m_Texture2DSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(m_Texture2DSpecification.FilterMode));
	}
}