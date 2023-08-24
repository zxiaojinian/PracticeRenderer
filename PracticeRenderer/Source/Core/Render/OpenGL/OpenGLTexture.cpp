#include "pch.h"
#include "OpenGLTexture.h"

#include "Core/Common.h"

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
			case PR::TextureFormat::Depth:
				return 4;
			default:
				return 4;
		}
	}

	GLenum TextureFormatToOpenGLInternalFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::Depth: return GL_DEPTH24_STENCIL8;
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
			case TextureFormat::Depth: 
				return GL_DEPTH_COMPONENT;
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

	GLenum TextureWrapModeToOpenGLWrapMode(TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
		case PR::TextureWrapMode::Clamp:
			return GL_CLAMP;
		case PR::TextureWrapMode::Repeat:
		default:
			return GL_REPEAT;
		}
	}

	GLenum TextureFilterToOpenGLFilterMode(TextureFilterMode filterMode, bool mipMap = false)
	{
		switch (filterMode)
		{
			case PR::TextureFilterMode::Nearest:
				return GL_NEAREST;
			case PR::TextureFilterMode::Bilinear:
				return GL_LINEAR;
			case PR::TextureFilterMode::Trilinear:
				if (mipMap)
					return GL_LINEAR_MIPMAP_LINEAR;
				else
					return GL_LINEAR;
			default:
				return GL_LINEAR;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_TextureSpecification(specification)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, TextureFormatToOpenGLInternalFormat(m_TextureSpecification.Format), m_TextureSpecification.Width, m_TextureSpecification.Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenGLWrapMode(m_TextureSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenGLWrapMode(m_TextureSpecification.WrapMode));

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGLFilterMode(m_TextureSpecification.FilterMode, m_TextureSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGLFilterMode(m_TextureSpecification.FilterMode));
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
			m_TextureSpecification.Width = width;
			m_TextureSpecification.Height = height;

			if (channels == 1)
				m_TextureSpecification.Format = TextureFormat::R;
			else if (channels == 3)
				m_TextureSpecification.Format = TextureFormat::RGB;
			else if (channels == 4)
				m_TextureSpecification.Format = TextureFormat::RGBA;
			else
				PR_ASSERT(false, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, TextureFormatToOpenGLInternalFormat(m_TextureSpecification.Format), m_TextureSpecification.Width, m_TextureSpecification.Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenGLWrapMode(m_TextureSpecification.WrapMode));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenGLWrapMode(m_TextureSpecification.WrapMode));

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGLFilterMode(m_TextureSpecification.FilterMode, m_TextureSpecification.GenerateMips));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGLFilterMode(m_TextureSpecification.FilterMode));

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_TextureSpecification.Width, m_TextureSpecification.Height, TextureFormatToOpenGLDataFormat(m_TextureSpecification.Format), GL_UNSIGNED_BYTE, data);

			if (m_TextureSpecification.GenerateMips)
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
		uint32_t bpp = TextureFormatToBPP(m_TextureSpecification.Format);
		PR_ASSERT(size == m_TextureSpecification.Width * m_TextureSpecification.Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_TextureSpecification.Width, m_TextureSpecification.Height, TextureFormatToOpenGLDataFormat(m_TextureSpecification.Format), GL_UNSIGNED_BYTE, data);

		if (m_TextureSpecification.GenerateMips)
			glGenerateTextureMipmap(m_RendererID);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetFilterMode(TextureFilterMode filterMode)
	{
		m_TextureSpecification.FilterMode = filterMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGLFilterMode(m_TextureSpecification.FilterMode, m_TextureSpecification.GenerateMips));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGLFilterMode(m_TextureSpecification.FilterMode));
	}

	void OpenGLTexture2D::SetWrapMode(TextureWrapMode wrapMode)
	{
		m_TextureSpecification.WrapMode = wrapMode;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenGLWrapMode(m_TextureSpecification.WrapMode));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenGLWrapMode(m_TextureSpecification.WrapMode));
	}

	void OpenGLTexture2D::SetGenerateMips(bool generateMips)
	{
		m_TextureSpecification.GenerateMips = generateMips;
		//TODO
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}