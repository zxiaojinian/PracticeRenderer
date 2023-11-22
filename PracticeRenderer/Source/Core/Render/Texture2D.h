#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	struct Texture2DSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureFormat Format = TextureFormat::R8G8B8A8_SRGB;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
		TextureFilterMode FilterMode = TextureFilterMode::Trilinear;
		bool GenerateMips = true;
	};

	class Texture2D : public Texture
	{
	public:
		virtual TextureDimension GetTextureDimension() const override { return TextureDimension::Tex2D; }


		virtual void SetData(void* data, uint32_t x, uint32_t y, uint32_t width, uint32_t height, TexturePixelType dataType) = 0;

		static std::shared_ptr<Texture2D> Create(const std::string& name, const Texture2DSpecification& specification);

		static void Init();
		static std::shared_ptr<Texture2D> GetWhiteTexture() { return m_WhiteTexture; }
		static std::shared_ptr<Texture2D> GetBlackTexture() { return m_BlackTexture; }
		static std::shared_ptr<Texture2D> GetGreyTexture() { return m_GreyTexture; }

	private:
		static std::shared_ptr<Texture2D> m_WhiteTexture;
		static std::shared_ptr<Texture2D> m_BlackTexture;
		static std::shared_ptr<Texture2D> m_GreyTexture;
	};
}

