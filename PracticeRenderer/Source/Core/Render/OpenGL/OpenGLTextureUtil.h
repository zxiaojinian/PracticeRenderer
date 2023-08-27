#pragma once

#include "Core/Render/Texture.h"

#include <glad/glad.h>

namespace PR
{
	inline GLenum TextureWrapModeToOpenGLWrapMode(TextureWrapMode wrapMode)
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

	inline GLenum TextureFilterToOpenGLFilterMode(TextureFilterMode filterMode, bool mipMap = false)
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
}