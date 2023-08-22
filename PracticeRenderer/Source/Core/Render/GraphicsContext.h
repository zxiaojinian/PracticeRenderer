#pragma once

#include "Core/Scene/Components/MeshRenderer.h"
#include "Core/Render/RenderData/CullingResults.h"
#include "Core/Render/RenderData/DrawingSettings.h"
#include "Core/Render/RenderData/FilteringSettings.h"

#include <glm/glm.hpp>

#include <memory>

namespace PR
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		void DrawRenderer(const MeshRenderer& renderer);
		void DrawRenderers(const CullingResults& cullingResults, const DrawingSettings& drawingSettings, const FilteringSettings& filteringSettings);

	public:
		static std::unique_ptr<GraphicsContext> Create();
	};
}
