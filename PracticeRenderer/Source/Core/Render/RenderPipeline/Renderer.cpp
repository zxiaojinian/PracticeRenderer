#include "pch.h"
#include "Renderer.h"

namespace PR
{
	void Renderer::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		std::sort(m_ActiveRenderPassQueue.begin(), m_ActiveRenderPassQueue.end(), std::less<std::shared_ptr<RenderPass>>());
		InternalStartRendering(renderingData);
		SetupLights(renderingData);

		RenderBlocks renderBlocks(m_ActiveRenderPassQueue);
		if(renderBlocks.GetLength(RenderPassBlock::BeforeRendering))
			ExecuteBlock(graphicsContext, RenderPassBlock::BeforeRendering, renderBlocks, renderingData);
		if (renderBlocks.GetLength(RenderPassBlock::MainRenderingOpaque))
			ExecuteBlock(graphicsContext, RenderPassBlock::MainRenderingOpaque, renderBlocks, renderingData);
		if (renderBlocks.GetLength(RenderPassBlock::MainRenderingTransparent))
			ExecuteBlock(graphicsContext, RenderPassBlock::MainRenderingTransparent, renderBlocks, renderingData);
		if (renderBlocks.GetLength(RenderPassBlock::AfterRendering))
			ExecuteBlock(graphicsContext, RenderPassBlock::AfterRendering, renderBlocks, renderingData);
		InternalFinishRendering();
	}

	void Renderer::EnqueuePass(std::shared_ptr<RenderPass> renderPass)
	{
		m_ActiveRenderPassQueue.push_back(renderPass);
	}

	void Renderer::ExecuteBlock(GraphicsContext& graphicsContext, RenderPassBlock blockIndex, const RenderBlocks& renderBlocks, const RenderingData& renderingData)
	{
		auto range = renderBlocks.GetRange(blockIndex);
		for (uint32_t i = range.first; i < range.second; ++i)
		{
			ExecuteRenderPass(graphicsContext, m_ActiveRenderPassQueue[i], renderingData);
		}
	}

	void Renderer::ExecuteRenderPass(GraphicsContext& graphicsContext, std::shared_ptr<RenderPass> renderPass, const RenderingData& renderingData)
	{
		renderPass->Configure();
		SetRenderPassAttachments(renderPass);
		renderPass->Execute(graphicsContext, renderingData);
	}

	void Renderer::SetRenderPassAttachments(std::shared_ptr<RenderPass> renderPass)
	{
	}

	void Renderer::InternalStartRendering(const RenderingData& renderingData)
	{
		for (auto& pass : m_ActiveRenderPassQueue)
		{
			pass->OnCameraSetup();
		}
	}

	void Renderer::InternalFinishRendering()
	{
		for (auto& pass : m_ActiveRenderPassQueue)
		{
			pass->OnCameraCleanup();
		}
		m_ActiveRenderPassQueue.clear();
	}
}
