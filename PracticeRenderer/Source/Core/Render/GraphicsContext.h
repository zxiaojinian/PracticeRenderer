#pragma once

#include "Core/Scene/Components/MeshRenderer.h"
#include "Core/Render/RenderData/CullingResults.h"
#include "Core/Render/RenderData/DrawingSettings.h"
#include "Core/Render/RenderData/FilteringSettings.h"
#include "Core/Render/RenderData/Color.h"
#include "Core/Render/RenderTexture.h"
#include "Core/Render/Framebuffer.h"
#include "Core/Event/EventData/BaseEvent.h"

#include <glm/glm.hpp>

#include <memory>

namespace PR
{
	class GraphicsContext
	{
	public:
		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;

		void Init();

		void SetRenderTarget(RenderTexture& color, RenderTexture& depth);
		void SetRenderTarget(std::vector<RenderTexture>& colors, RenderTexture& depth);
		void SetBackBuffer();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void SetViewport(glm::u32vec4 viewPort);
		void ClearRenderTarget(bool clearDepth, bool clearColor, Color& backgroundColor = Color::clear);
		void DrawMesh(Mesh& mesh, const glm::mat4& matrix, Material& material);
		void DrawRenderer(const MeshRenderer& renderer);
		void DrawRenderers(const CullingResults& cullingResults, const DrawingSettings& drawingSettings, const FilteringSettings& filteringSettings);
	public:
		std::unique_ptr<Framebuffer> m_Framebuffer;
		static std::unique_ptr<GraphicsContext> Create();
	};
}
