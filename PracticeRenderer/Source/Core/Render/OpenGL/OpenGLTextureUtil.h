#pragma once

#include "Core/Render/Texture.h"
#include "Core/Render/Texture2D.h"
#include "Core/Render/Cubemap.h"

#include <glad/glad.h>

namespace PR
{
	inline GLenum GetOpenGLWrapMode(TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
			case PR::TextureWrapMode::Clamp:
				return GL_CLAMP_TO_EDGE;
			case PR::TextureWrapMode::Repeat:
			default:
				return GL_REPEAT;
		}
	}

	inline GLenum GetOpenGLFilterMode(TextureFilterMode filterMode, bool mipMap = false)
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

	inline GLenum GetOpenGLInternalFormat(TextureFormat format)
	{
		GLenum internalFormat;

		switch (format)
		{
			case TextureFormat::R8G8B8_SRGB:
				internalFormat = GL_SRGB8;
				break;
			case TextureFormat::R8G8B8A8_SRGB:
				internalFormat = GL_SRGB8_ALPHA8;
				break;
			case TextureFormat::R8_UNorm:
				internalFormat = GL_R8;
				break;
			case TextureFormat::R8G8B8_UNorm:
				internalFormat = GL_RGB8;
				break;
			case TextureFormat::R8G8B8A8_UNorm:
				internalFormat = GL_RGBA8;
				break;
			case TextureFormat::R16_SFloat:
				internalFormat = GL_R16F;
				break;
			case TextureFormat::R16G16B16_SFloat:
				internalFormat = GL_RGB16F;
				break;
			case TextureFormat::R16G16B16A16_SFloat:
				internalFormat = GL_RGBA16F;
				break;
			case TextureFormat::R32_SFloat:
				internalFormat = GL_R32F;
				break;
			case TextureFormat::R32G32B32_SFloat:
				internalFormat = GL_RGB32F;
				break;
			case TextureFormat::R32G32B32A32_SFloat:
				internalFormat = GL_RGBA32F;
				break;
			case TextureFormat::R11G11B10_UFloatPack32:
				internalFormat = GL_R11F_G11F_B10F;
				break;
			case TextureFormat::D16_UNorm:
				internalFormat = GL_DEPTH_COMPONENT16;
				break;
			case TextureFormat::D24_UNorm:
				internalFormat = GL_DEPTH_COMPONENT24;
				break;
			case TextureFormat::D24_UNorm_S8_UInt:
				internalFormat = GL_DEPTH24_STENCIL8;
				break;
			case TextureFormat::D32_SFloat:
				internalFormat = GL_DEPTH_COMPONENT32F;
				break;
			case TextureFormat::D32_SFloat_S8_UInt:
				internalFormat = GL_DEPTH32F_STENCIL8;
				break;
			default:
				internalFormat = GL_SRGB8_ALPHA8;
				break;
		}

		return internalFormat;
	}

	inline GLenum GetOpenGLPixelFormat(TextureFormat format)
	{
		GLenum dataFormat;

		switch (format)
		{
			case TextureFormat::R8_UNorm:
			case TextureFormat::R16_SFloat:
			case TextureFormat::R32_SFloat:
				dataFormat = GL_RED;
				break;
			case TextureFormat::R8G8B8A8_SRGB:
			case TextureFormat::R8G8B8A8_UNorm:
			case TextureFormat::R16G16B16A16_SFloat:
			case TextureFormat::R32G32B32A32_SFloat:
				dataFormat = GL_RGBA;
				break;
			case TextureFormat::R8G8B8_SRGB:
			case TextureFormat::R8G8B8_UNorm:
			case TextureFormat::R16G16B16_SFloat:
			case TextureFormat::R32G32B32_SFloat:
			case TextureFormat::R11G11B10_UFloatPack32:
				dataFormat = GL_RGB;
				break;
			case TextureFormat::D16_UNorm:
			case TextureFormat::D24_UNorm:
			case TextureFormat::D32_SFloat:
				dataFormat = GL_DEPTH_COMPONENT;
				break;
			case TextureFormat::D24_UNorm_S8_UInt:
			case TextureFormat::D32_SFloat_S8_UInt:
				dataFormat = GL_DEPTH_STENCIL;
				break;
			default:
				dataFormat = GL_RGBA;
				break;
		}

		return dataFormat;
	}

	inline GLenum GetOpenGLPixelType(TexturePixelType dataType)
	{
		GLenum resultDataType;

		switch (dataType)
		{
		case TexturePixelType::UNSIGNED_BYTE:
				resultDataType = GL_UNSIGNED_BYTE;
				break;
			case TexturePixelType::BYTE:
				resultDataType = GL_BYTE;
				break;
			case TexturePixelType::UNSIGNED_SHORT:
				resultDataType = GL_UNSIGNED_SHORT;
				break;
			case TexturePixelType::SHORT:
				resultDataType = GL_SHORT;
				break;
			case TexturePixelType::UNSIGNED_INT:
				resultDataType = GL_UNSIGNED_INT;
				break;
			case TexturePixelType::INT:
				resultDataType = GL_INT;
				break;
			case TexturePixelType::FLOAT:
				resultDataType = GL_FLOAT;
				break;
			default:
				resultDataType = GL_UNSIGNED_BYTE;
				break;
		}

		return resultDataType;
	}


	inline GLenum GetOpenGLCubemapFace(CubemapFace face)
	{
		switch (face)
		{
			case CubemapFace::PositiveX: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			case CubemapFace::NegativeX: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			case CubemapFace::PositiveY: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			case CubemapFace::NegativeY: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			case CubemapFace::PositiveZ: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			case CubemapFace::NegativeZ: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		}

		return 0;
	}
}