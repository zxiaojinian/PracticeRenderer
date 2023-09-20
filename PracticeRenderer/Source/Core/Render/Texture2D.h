#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	enum class TextureFormat
	{
		R,
		RGB,
		RGBA,
	};

	struct Texture2DSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureFormat Format = TextureFormat::RGBA;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
		TextureFilterMode FilterMode = TextureFilterMode::Bilinear;
		bool SRGB = false;
		bool GenerateMips = true;
	};

	class Texture2D : public Texture
	{
	public:
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual TextureFormat GetFormat() const = 0;

		static std::shared_ptr<Texture2D> Create(const std::string& name, const Texture2DSpecification& specification);
		static std::shared_ptr<Texture2D> Create(const std::string& path);

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

