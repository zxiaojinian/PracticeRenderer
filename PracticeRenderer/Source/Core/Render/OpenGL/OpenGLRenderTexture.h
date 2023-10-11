#pragma once

#include "Core/Render/RenderTexture.h"

namespace PR
{
	class OpenGLRenderTexture : public RenderTexture
	{
	public:
		OpenGLRenderTexture(const std::string& name, const RenderTextureSpecification& specification);
		virtual ~OpenGLRenderTexture();

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void SetFilterMode(TextureFilterMode filterMode) override;
		virtual void SetWrapMode(TextureWrapMode wrapMode) override;
		virtual void SetTextureFormat(TextureFormat format) override;
		virtual void SetGenerateMips(bool generateMips) override;

		virtual const std::string& GetName() const { return m_Name; }
		virtual uint32_t GetWidth() const override { return m_RenderTextureSpecification.Width; }
		virtual uint32_t GetHeight() const override { return m_RenderTextureSpecification.Height; }
		virtual TextureFormat GetFormat() const override { return m_RenderTextureSpecification.Format; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void BindImage(uint32_t slot = 0, uint32_t level = 0, TextureAccess access = TextureAccess::Read) const override;
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLRenderTexture&)other).m_RendererID;
		}

	private:
		void Init();

	private:
		std::string m_Name;
		RenderTextureSpecification m_RenderTextureSpecification;
		uint32_t m_RendererID;
	};
}


