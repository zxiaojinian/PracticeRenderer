#include "pch.h"
#include "PostProcessPass.h"

namespace PR
{
	PostProcessPass::PostProcessPass(RenderPassEvent renderPassEvent, std::shared_ptr<Material>& blitMaterial) : RenderPass(renderPassEvent)
	{
		m_BlitMaterial = blitMaterial;
	}

	void PostProcessPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		uint32_t w = renderingData.cameraData.pixelRect.z / 2;
		uint32_t h = renderingData.cameraData.pixelRect.w / 2;

		uint32_t mipMapCount = static_cast<uint32_t>( std::floor(std::log2( (std::max)(w, h) )) );
		const uint32_t minMipCount = 0;
		mipMapCount = std::clamp(mipMapCount, minMipCount, k_MaxMipCount);
		if (w != m_InitWidth || h != m_InitHeight)
		{
			m_InitWidth = w;
			m_InitHeight = h;

			m_BloomMipTexture.clear();
			RenderTextureSpecification colorSpecification = { w, h, 1, TextureFormat::R11G11B10_UFloatPack32, TextureWrapMode::Clamp, TextureFilterMode::Bilinear, false };
			for (uint32_t mipIndex = 0; mipIndex < mipMapCount; ++mipIndex)
			{
				colorSpecification.Width = w;
				colorSpecification.Height = h;
				w /= 2;
				h /= 2;
				auto mipTexture = RenderTexture::Create("mipTex" + mipIndex, colorSpecification);
				m_BloomMipTexture.push_back(mipTexture);
			}
		}
		//draw fullscreen meshs
		graphicsContext.SetBackBuffer();
		graphicsContext.SetViewport(renderingData.cameraData.pixelRect);
		Shader::SetTexture("u_SourceTex", m_Source.get());
		graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_BlitMaterial);
	}

	void PostProcessPass::Setup(std::shared_ptr<RenderTexture>& source)
	{
		m_Source = source;
	}
}
