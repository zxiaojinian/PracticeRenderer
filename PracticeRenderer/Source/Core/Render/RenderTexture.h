#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	enum class RenderTextureFormat
	{
		R,
		RGB,
		RGBA,
		Depth,
	};

	struct RenderTextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		RenderTextureFormat Format = RenderTextureFormat::RGBA;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
		TextureFilterMode FilterMode = TextureFilterMode::Bilinear;
		bool SRGB = false;
		bool GenerateMips = true;
	};

	class RenderTexture : public Texture
	{
	public:
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual RenderTextureFormat GetFormat() const = 0;

		static std::shared_ptr<RenderTexture> Create(const std::string& name, const RenderTextureSpecification& specification);
	};
}

