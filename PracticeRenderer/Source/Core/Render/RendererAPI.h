#pragma once

#include "Core/Render/VertexArray.h"

#include <glm/glm.hpp>

#include <memory>

namespace PR
{
	class RendererAPI
	{
	public:
		enum class API 
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(uint32_t indexCount = 0) = 0;

		static API GetAPI() { return s_API; }
		static std::unique_ptr<RendererAPI> Create();

	private:
		static API s_API;
	};
}

