#pragma once

#include "Core/Scene/Components/MeshRenderer.h"
#include "Core/Render/RenderData/CullingResults.h"
#include "Core/Render/RenderData/DrawingSettings.h"
#include "Core/Render/RenderData/FilteringSettings.h"
#include "Core/Render/RenderData/Color.h"

#include <glm/glm.hpp>

#include <memory>

namespace PR
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		void SetRenderTarget();
		void ClearRenderTarget(bool clearDepth, bool clearColor, Color& backgroundColor = Color::clear);
		void DrawRenderer(const MeshRenderer& renderer);
		void DrawRenderers(const CullingResults& cullingResults, const DrawingSettings& drawingSettings, const FilteringSettings& filteringSettings);

	public:
		static std::unique_ptr<GraphicsContext> Create();
	};
}
