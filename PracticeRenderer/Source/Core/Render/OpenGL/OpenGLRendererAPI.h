#pragma once

#include "Core/Render/RendererAPI.h"

#include <memory>

namespace PR
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetDepthCompareFunction(const CompareFunction& compareFunction) override;
		virtual void SetDepthWriteEnabled(bool enbale) override;
		virtual void SetCullMode(const CullMode& cullMode) override;
		virtual void SetBlendState(const BlendState& blendState) override;
		virtual void SetColorMask(const ColorWriteMask& colorMask) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const Color& color) override;
		virtual void Clear(bool clearDepth, bool clearColor) override;
		virtual void DrawIndexed(uint32_t indexCount = 0) override;
		virtual void DispatchCompute(uint32_t num_x, uint32_t num_y, uint32_t num_z) override;
	};
}

