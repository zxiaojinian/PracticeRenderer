#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	enum class CubemapFace : uint8_t
	{
		PositiveX,
		NegativeX,
		PositiveY,
		NegativeY,
		PositiveZ,
		NegativeZ,

		Count
	};

	struct CubemapSpecification
	{
		uint32_t Width = 1;
		TextureFormat Format = TextureFormat::R8G8B8A8_SRGB;
		TextureWrapMode WrapMode = TextureWrapMode::Clamp;
		TextureFilterMode FilterMode = TextureFilterMode::Bilinear;
		bool GenerateMips = false;
	};

	class Cubemap : public Texture
	{
	public:
		virtual void SetSmoothEdges(bool smooth) = 0;
		virtual void SetData(void* data, uint32_t x, uint32_t y, uint32_t width, uint32_t height, TexturePixelType dataType, CubemapFace face) = 0;

		static std::shared_ptr<Cubemap> Create(const std::string& name, const CubemapSpecification& specification);
	};
}

