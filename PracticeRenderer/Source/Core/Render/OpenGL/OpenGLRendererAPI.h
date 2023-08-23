#pragma once

#include "Core/Render/RendererAPI.h"

#include <memory>

namespace PR
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const Color& color) override;
		virtual void Clear(bool clearDepth, bool clearColor) override;

		virtual void DrawIndexed(uint32_t indexCount = 0) override;
	};
}

