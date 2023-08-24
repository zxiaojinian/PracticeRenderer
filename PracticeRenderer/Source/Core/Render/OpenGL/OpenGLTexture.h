#pragma once

#include "Core/Render/Texture2D.h"

#include <glad/glad.h>

#include <string>

namespace PR
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const TextureSpecification& specification);
		virtual ~OpenGLTexture2D();

		virtual void SetFilterMode(TextureFilterMode filterMode) override;
		virtual void SetWrapMode(TextureWrapMode wrapMode) override;
		virtual void SetGenerateMips(bool generateMips) override;
		virtual const std::string& GetName() const { return m_Name; }
		virtual uint32_t GetWidth() const override { return m_TextureSpecification.Width; }
		virtual uint32_t GetHeight() const override { return m_TextureSpecification.Height; }
		virtual TextureFormat GetFormat() const override { return m_TextureSpecification.Format; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		std::string m_Name;
		std::string m_Path;
		TextureSpecification m_TextureSpecification;
		uint32_t m_RendererID;
	};
}

