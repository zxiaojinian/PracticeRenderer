#include "pch.h"
#include "OpenGLTexture.h"

#include "Core/Common.h"
#include "OpenGLTextureUtil.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace PR
{
	uint32_t TextureFormatToBPP(TextureFormat format)
	{
		switch (format)
		{
		case PR::TextureFormat::R:
			return 1;
		case PR::TextureFormat::RGB:
			return 3;
		case PR::TextureFormat::RGBA:
			return 4;
		default:
			return 4;
		}
	}

	GLenum TextureFormatToOpenGLInternalFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::R: return GL_R8;
			case TextureFormat::RGB: return GL_RGB8;
			case TextureFormat::RGBA: return GL_RGBA8;
			default: return GL_RGBA8;
		}
	}

	GLenum TextureFormatToOpenGLDataFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::R:
				return GL_R;
			case TextureFormat::RGB:
				return GL_RGB;
			case TextureFormat::RGBA:
				return GL_RGBA;
			default:
				return GL_RGBA;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const Texture2DSpecification& specification)
		: m_Texture2DSpecification(specification)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 0, TextureFormatToOpenGLInternalFormat(m_Texture2DSpecification.Format), m_Texture2DSpecification.Width, m_Texture2DSpecification.Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenGLWrapMode(m_Texture2DSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenGLWrapMode(m_Texture2DSpecification.WrapMode));

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGLFilterMode(m_Texture2DSpecification.FilterMode, m_Texture2DSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGLFilterMode(m_Texture2DSpecification.FilterMode));
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		PR_ASSERT(data, "Failed to load texture: {0}", path);

		if (data)
		{
			m_Texture2DSpecification.Width = width;
			m_Texture2DSpecification.Height = height;

			if (channels == 1)
				m_Texture2DSpecification.Format = TextureFormat::R;
			else if (channels == 3)
				m_Texture2DSpecification.Format = TextureFormat::RGB;
			else if (channels == 4)
				m_Texture2DSpecification.Format = TextureFormat::RGBA;
			else
				PR_ASSERT(false, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 0, TextureFormatToOpenGLInternalFormat(m_Texture2DSpecification.Format), m_Texture2DSpecification.Width, m_Texture2DSpecification.Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenGLWrapMode(m_Texture2DSpecification.WrapMode));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenGLWrapMode(m_Texture2DSpecification.WrapMode));

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGLFilterMode(m_Texture2DSpecification.FilterMode, m_Texture2DSpecification.GenerateMips));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGLFilterMode(m_Texture2DSpecification.FilterMode));

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Texture2DSpecification.Width, m_Texture2DSpecification.Height, TextureFormatToOpenGLDataFormat(m_Texture2DSpecification.Format), GL_UNSIGNED_BYTE, data);

			if (m_Texture2DSpecification.GenerateMips)
				glGenerateTextureMipmap(m_RendererID);

			stbi_image_free(data);
		}
		else
		{
			PR_LOG_ERROR("Failed to load texture {0}", path);
			m_RendererID = 0;
		}
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = TextureFormatToBPP(m_Texture2DSpecification.Format);
		PR_ASSERT(size == m_Texture2DSpecification.Width * m_Texture2DSpecification.Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Texture2DSpecification.Width, m_Texture2DSpecification.Height, TextureFormatToOpenGLDataFormat(m_Texture2DSpecification.Format), GL_UNSIGNED_BYTE, data);

		if (m_Texture2DSpecification.GenerateMips)
			glGenerateTextureMipmap(m_RendererID);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetFilterMode(TextureFilterMode filterMode)
	{
		m_Texture2DSpecification.FilterMode = filterMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGLFilterMode(m_Texture2DSpecification.FilterMode, m_Texture2DSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGLFilterMode(m_Texture2DSpecification.FilterMode));
	}

	void OpenGLTexture2D::SetWrapMode(TextureWrapMode wrapMode)
	{
		m_Texture2DSpecification.WrapMode = wrapMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenGLWrapMode(m_Texture2DSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenGLWrapMode(m_Texture2DSpecification.WrapMode));
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

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}