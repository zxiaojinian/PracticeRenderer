#pragma once

#include <string>
#include <memory>

namespace PR
{
	enum class TextureFormat
	{
		R,
		RGB,
		RGBA,
		Depth,
	};

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

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureFormat Format = TextureFormat::RGBA;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
		TextureFilterMode FilterMode = TextureFilterMode::Bilinear;
		bool SRGB = false;
		bool GenerateMips = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void SetFilterMode(TextureFilterMode filterMode) = 0;
		virtual void SetWrapMode(TextureWrapMode wrapMode) = 0;
		virtual void SetGenerateMips(bool generateMips) = 0;
		virtual const std::string& GetName() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual TextureFormat GetFormat() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;

	};
}
