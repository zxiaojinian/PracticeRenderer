#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	struct RenderTextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureFormat Format = TextureFormat::R11G11B10_UFloatPack32;
		TextureWrapMode WrapMode = TextureWrapMode::Clamp;
		TextureFilterMode FilterMode = TextureFilterMode::Bilinear;
		bool GenerateMips = false;
	};

	class RenderTexture : public Texture
	{
	public:
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static std::shared_ptr<RenderTexture> Create(const std::string& name, const RenderTextureSpecification& specification);
	};
}

