#include "pch.h"
#include "Renderer.h"

namespace PR
{
	void Renderer::Execute(const RenderingData& renderingData)
	{
		InternalStartRendering(renderingData);

		std::sort(m_ActiveRenderPassQueue.begin(), m_ActiveRenderPassQueue.end(), std::less<std::shared_ptr<RenderPass>>());

		SetupLights(renderingData);

		RenderBlocks renderBlocks(m_ActiveRenderPassQueue);
		if(renderBlocks.GetLength(RenderPassBlock::BeforeRendering))
			ExecuteBlock(RenderPassBlock::BeforeRendering, renderBlocks, renderingData);
		if (renderBlocks.GetLength(RenderPassBlock::MainRenderingOpaque))
			ExecuteBlock(RenderPassBlock::MainRenderingOpaque, renderBlocks, renderingData);
		if (renderBlocks.GetLength(RenderPassBlock::MainRenderingTransparent))
			ExecuteBlock(RenderPassBlock::MainRenderingTransparent, renderBlocks, renderingData);
		if (renderBlocks.GetLength(RenderPassBlock::AfterRendering))
			ExecuteBlock(RenderPassBlock::AfterRendering, renderBlocks, renderingData);
		InternalFinishRendering();
	}

	void Renderer::EnqueuePass(std::shared_ptr<RenderPass> renderPass)
	{
		m_ActiveRenderPassQueue.push_back(renderPass);
	}

	void Renderer::ExecuteBlock(RenderPassBlock blockIndex, const RenderBlocks& renderBlocks, const RenderingData& renderingData)
	{
		auto range = renderBlocks.GetRange(blockIndex);
		for (uint32_t i = range.first; i < range.second; ++i)
		{
			ExecuteRenderPass(m_ActiveRenderPassQueue[i], renderingData);
		}
	}

	void Renderer::ExecuteRenderPass(std::shared_ptr<RenderPass> renderPass, const RenderingData& renderingData)
	{
		renderPass->Configure();
		SetRenderPassAttachments(renderPass);
		renderPass->Execute(renderingData);
	}

	void Renderer::SetRenderPassAttachments(std::shared_ptr<RenderPass> renderPass)
	{
	}

	void Renderer::InternalStartRendering(const RenderingData& renderingData)
	{
	}

	void Renderer::InternalFinishRendering()
	{
	}
}
