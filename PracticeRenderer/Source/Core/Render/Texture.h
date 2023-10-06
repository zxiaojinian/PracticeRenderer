#pragma once

#include <string>
#include <memory>

namespace PR
{
	enum class TextureWrapMode
	{
		Clamp,
		Repeat
	};

	enum class TextureFilterMode
	{
		Nearest,
		Bilinear,
		Trilinear
	};

	enum class TextureFormat
	{
		R8G8B8_SRGB,
		R8G8B8A8_SRGB,
		R8_UNorm,
		R8G8B8_UNorm,
		R8G8B8A8_UNorm,
		R16_SFloat,
		R16G16B16_SFloat,
		R16G16B16A16_SFloat,
		R32_SFloat,
		R32G32B32_SFloat,
		R32G32B32A32_SFloat,
		R11G11B10_UFloatPack32,
		D16_UNorm,
		D24_UNorm,
		D24_UNorm_S8_UInt,
		D32_SFloat,
		D32_SFloat_S8_UInt
	};

	enum TexturePixelType
	{
		UNSIGNED_BYTE,
		BYTE,
		UNSIGNED_SHORT,
		SHORT,
		UNSIGNED_INT,
		INT,
		FLOAT
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void SetFilterMode(TextureFilterMode filterMode) = 0;
		virtual void SetWrapMode(TextureWrapMode wrapMode) = 0;
		virtual void SetTextureFormat(TextureFormat format) = 0;
		virtual void SetGenerateMips(bool generateMips) = 0;

		virtual const std::string& GetName() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual TextureFormat GetFormat() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;
	};
}
