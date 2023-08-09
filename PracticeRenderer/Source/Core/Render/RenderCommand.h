#pragma once

#include "Core/Render/RendererAPI.h"
#include "Core/Render/VertexArray.h"

#include <glm/glm.hpp>

#include <memory>

namespace PR
{
	class RenderCommand
	{
	public:
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};
}
