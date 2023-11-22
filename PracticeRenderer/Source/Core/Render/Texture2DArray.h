#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	struct Texture2DArraySpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Slice = 1;
		TextureFormat Format = TextureFormat::R8G8B8A8_SRGB;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
		TextureFilterMode FilterMode = TextureFilterMode::Trilinear;
		bool GenerateMips = true;

	};
	class Texture2DArray : public Texture
	{
	public:
		virtual TextureDimension GetTextureDimension() const override { return TextureDimension::Tex2DArray; }

		virtual void SetData(void* data, uint32_t x, uint32_t y, uint32_t z, uint32_t width, uint32_t height, uint32_t slice, TexturePixelType dataType) = 0;

		static std::shared_ptr<Texture2DArray> Create(const std::string& name, const Texture2DArraySpecification& specification);
	};
}

