#pragma once

#include "Core/Render/RenderTexture.h"

namespace PR
{
	class OpenGLRenderTexture : public RenderTexture
	{
	public:
		OpenGLRenderTexture(const RenderTextureSpecification& specification);
		virtual ~OpenGLRenderTexture();

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void SetFilterMode(TextureFilterMode filterMode) override;
		virtual void SetWrapMode(TextureWrapMode wrapMode) override;
		virtual void SetGenerateMips(bool generateMips) override;
		virtual const std::string& GetName() const { return m_Name; }
		virtual uint32_t GetWidth() const override { return m_RenderTextureSpecification.Width; }
		virtual uint32_t GetHeight() const override { return m_RenderTextureSpecification.Height; }
		virtual RenderTextureFormat GetFormat() const override { return m_RenderTextureSpecification.Format; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLRenderTexture&)other).m_RendererID;
		}
	private:
		std::string m_Name;
		RenderTextureSpecification m_RenderTextureSpecification;
		uint32_t m_RendererID;
	};
}


