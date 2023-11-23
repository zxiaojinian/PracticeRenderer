#pragma once

#include "Core/Render/Cubemap.h"

namespace PR
{

	class OpenGLCubemap : public Cubemap
	{
	public:
		OpenGLCubemap(const std::string& name, const CubemapSpecification& specification);
		virtual ~OpenGLCubemap();

		virtual void SetFilterMode(TextureFilterMode filterMode) override;
		virtual void SetWrapMode(TextureWrapMode wrapMode) override;
		virtual void SetTextureFormat(TextureFormat format) override;
		virtual void SetGenerateMips(bool generateMips) override;

		virtual const std::string& GetName() const { return m_Name; }
		virtual uint32_t GetWidth() const override { return m_CubemapSpecification.Width; }
		virtual uint32_t GetHeight() const override { return m_CubemapSpecification.Width; }
		virtual TextureFormat GetFormat() const override { return m_CubemapSpecification.Format; }
		virtual void EnableCompare() override;
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetSmoothEdges(bool smooth) override { m_SmoothEdges = smooth; }
		virtual void SetData(void* data, uint32_t x, uint32_t y, uint32_t width, uint32_t height, TexturePixelType dataType, CubemapFace face) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void BindImage(uint32_t slot = 0, uint32_t level = 0, TextureAccess access = TextureAccess::Read, uint32_t slice = 0) const override;
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLCubemap&)other).m_RendererID;
		}

	private:
		void Init();

	private:
		std::string m_Name;
		CubemapSpecification m_CubemapSpecification;
		uint32_t m_RendererID;
		bool m_SmoothEdges = false;
	};
}

