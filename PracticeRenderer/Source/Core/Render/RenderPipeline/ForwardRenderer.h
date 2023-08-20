#pragma once

#include "Core/Render/RenderPipeline/Renderer.h"
#include "Core/Render/RenderPipeline/RenderPass/DrawObjectsPass.h"

namespace PR
{
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer();

		virtual void Setup(RenderingData& renderingData) override;

	private:
		std::shared_ptr<DrawObjectsPass> m_DrawObjectsPass;
	};
}

