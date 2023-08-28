#include "pch.h"
#include "DrawObjectsPass.h"

#include "Core/Render/RenderData/DrawingSettings.h"
#include "Core/Render/RenderData/FilteringSettings.h"

namespace PR
{
	DrawObjectsPass::DrawObjectsPass()
	{
		m_ColorRenderTexture = RenderTexture::Create({ 1920, 1080 });
		m_DepthRenderTexture = RenderTexture::Create({ 1920, 1080, RenderTextureFormat::Depth, TextureWrapMode::Clamp, TextureFilterMode::Nearest, false, false });
	}

	void DrawObjectsPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		graphicsContext.SetRenderTarget(*m_ColorRenderTexture, *m_DepthRenderTexture);

		DrawingSettings drawingSettings;
		FilteringSettings filteringSettings;
		graphicsContext.DrawRenderers(renderingData.cullResults, drawingSettings, filteringSettings);
	}
}
