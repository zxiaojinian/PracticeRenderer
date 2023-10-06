#pragma once

#include "Core/Render/Texture2D.h"

#include <glad/glad.h>

#include <string>

namespace PR
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& name, const Texture2DSpecification& specification);
		virtual ~OpenGLTexture2D();

		virtual void SetFilterMode(TextureFilterMode filterMode) override;
		virtual void SetWrapMode(TextureWrapMode wrapMode) override;
		virtual void SetTextureFormat(TextureFormat format) override;
		virtual void SetGenerateMips(bool generateMips) override;

		virtual const std::string& GetName() const { return m_Name; }
		virtual uint32_t GetWidth() const override { return m_Texture2DSpecification.Width; }
		virtual uint32_t GetHeight() const override { return m_Texture2DSpecification.Height; }
		virtual TextureFormat GetFormat() const override { return m_Texture2DSpecification.Format; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t x, uint32_t y, uint32_t width, uint32_t height, TexturePixelType dataType) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}

	private:
		void Init();

	private:
		std::string m_Name;
		Texture2DSpecification m_Texture2DSpecification;
		uint32_t m_RendererID;
	};
}

