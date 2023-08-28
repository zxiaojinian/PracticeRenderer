#pragma once

#include "Core/Scene/Components/MeshRenderer.h"
#include "Core/Render/RenderData/CullingResults.h"
#include "Core/Render/RenderData/DrawingSettings.h"
#include "Core/Render/RenderData/FilteringSettings.h"
#include "Core/Render/RenderData/Color.h"
#include "Core/Render/RenderTexture.h"
#include "Core/Render/Framebuffer.h"

#include <glm/glm.hpp>

#include <memory>

namespace PR
{
	class GraphicsContext
	{
	public:
		GraphicsContext();
		virtual ~GraphicsContext() = default;

		void SetRenderTarget(RenderTexture& color, RenderTexture& depth);
		void SetRenderTarget(std::vector<RenderTexture&>& colors, RenderTexture& depth);
		void ClearRenderTarget(bool clearDepth, bool clearColor, Color& backgroundColor = Color::clear);
		void DrawRenderer(const MeshRenderer& renderer);
		void DrawRenderers(const CullingResults& cullingResults, const DrawingSettings& drawingSettings, const FilteringSettings& filteringSettings);

	public:
		std::unique_ptr<Framebuffer> m_Framebuffer;

		static std::unique_ptr<GraphicsContext> Create();
	};
}
