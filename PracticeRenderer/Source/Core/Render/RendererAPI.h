#pragma once

#include "Core/Render/VertexArray.h"
#include "Core/Render/RenderData/Color.h"
#include "Core/Render/RenderState/CompareFunction.h"
#include "Core/Render/RenderState/CullMode.h"
#include "Core/Render/RenderState/ColorWriteMask.h"
#include "Core/Render/RenderState/BlendState.h"

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

		virtual void SetDepthCompareFunction(const CompareFunction& compareFunction) = 0;
		virtual void SetDepthWriteEnabled(bool enbale) = 0;
		virtual void SetCullMode(const CullMode& cullMode) = 0;
		virtual void SetBlendState(const BlendState& blendState) = 0;
		virtual void SetColorMask(const ColorWriteMask& colorMask) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const Color& color) = 0;
		virtual void Clear(bool clearDepth, bool clearColor) = 0;
		virtual void DrawIndexed(uint32_t indexCount = 0) = 0;
		virtual void DispatchCompute(uint32_t num_x, uint32_t num_y, uint32_t num_z) = 0;

		static API GetAPI() { return s_API; }
		static std::unique_ptr<RendererAPI> Create();

	private:
		static API s_API;
	};
}

