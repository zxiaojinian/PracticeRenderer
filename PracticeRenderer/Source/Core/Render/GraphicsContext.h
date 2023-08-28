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
		void ClearRenderTarget(bool clearDepth, bool clearColor, Color& backgroundColor = Color::clear);
		void DrawRenderer(const MeshRenderer& renderer);
		void DrawRenderers(const CullingResults& cullingResults, const DrawingSettings& drawingSettings, const FilteringSettings& filteringSettings);

		void OnWindowResize(Event& e);
	public:
		std::unique_ptr<Framebuffer> m_Framebuffer;
		
		struct ViewPort
		{
			uint32_t x{ 0 };
			uint32_t y{ 0 };
			uint32_t width{ 1 };
			uint32_t height{ 1 };
		};

		ViewPort m_ViewPort;

		static std::unique_ptr<GraphicsContext> Create();
	};
}
