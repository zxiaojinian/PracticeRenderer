#include "pch.h"
#include "DrawObjectsPass.h"

#include "Core/Render/RenderData/DrawingSettings.h"
#include "Core/Render/RenderData/FilteringSettings.h"

namespace PR
{
	void DrawObjectsPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		DrawingSettings drawingSettings;
		FilteringSettings filteringSettings;
		graphicsContext.DrawRenderers(renderingData.cullResults, drawingSettings, filteringSettings);
	}
}
