#pragma once

#include "Core/Render/RendererAPI.h"
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
	class RenderCommand
	{
	public:
		static void SetDepthCompareFunction(const CompareFunction& compareFunction)
		{
			s_RendererAPI->SetDepthCompareFunction(compareFunction);
		}

		static void SetDepthWriteEnabled(bool enbale)
		{
			s_RendererAPI->SetDepthWriteEnabled(enbale);
		}

		static void SetCullMode(const CullMode& cullMode)
		{
			s_RendererAPI->SetCullMode(cullMode);
		}

		static void SetBlendState(const BlendState& blendState)
		{
			s_RendererAPI->SetBlendState(blendState);
		}

		static void SetColorMask(const ColorWriteMask& colorMask)
		{
			s_RendererAPI->SetColorMask(colorMask);
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const Color& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear(bool clearDepth, bool clearColor)
		{
			s_RendererAPI->Clear(clearDepth, clearColor);
		}

		static void DrawIndexed(uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(count);
		}

		static void DispatchCompute(uint32_t num_x, uint32_t num_y, uint32_t num_z)
		{
			s_RendererAPI->DispatchCompute(num_x, num_y, num_z);
		}

	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};
}

