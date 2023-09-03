#include "pch.h"
#include "DrawObjectsPass.h"

#include "Core/Render/RenderData/DrawingSettings.h"
#include "Core/Render/RenderData/FilteringSettings.h"

#include <glad/glad.h>

namespace PR
{
	DrawObjectsPass::DrawObjectsPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{

	}

	void DrawObjectsPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		DrawingSettings drawingSettings;
		FilteringSettings filteringSettings;

		glEnable(GL_DEPTH_TEST);
		graphicsContext.DrawRenderers(renderingData.cullResults, drawingSettings, filteringSettings);
	}
}
