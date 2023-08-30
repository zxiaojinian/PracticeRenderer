#include "pch.h"
#include "Renderer.h"

namespace PR
{
	void Renderer::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		std::sort(m_ActiveRenderPassQueue.begin(), m_ActiveRenderPassQueue.end(), [](const std::shared_ptr<RenderPass>& l, const std::shared_ptr<RenderPass>& r){
				return *l < *r;
			});
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
		SetRenderPassAttachments(graphicsContext, renderPass);
		renderPass->Execute(graphicsContext, renderingData);
	}

	void Renderer::ConfigureCameraTarget(std::shared_ptr<RenderTexture>& colorTarget, std::shared_ptr<RenderTexture>& depthTarget)
	{
		m_CameraColorTarget = colorTarget;
		m_CameraDepthTarget = depthTarget;
	}

	void Renderer::SetRenderPassAttachments(GraphicsContext& graphicsContext, std::shared_ptr<RenderPass> renderPass)
	{
		std::shared_ptr<RenderTexture>& passColorAttachment = renderPass->GetColorAttachment();
		std::shared_ptr<RenderTexture>& passDepthAttachment = renderPass->GetDepthAttachment();

		if (!renderPass->OverrideCameraTarget)
		{
			if (renderPass->renderPassEvent < RenderPassEvent::BeforeRenderingOpaques)
				return;

			passColorAttachment = m_CameraColorTarget;
			passDepthAttachment = m_CameraDepthTarget;
		}

		if (passColorAttachment != m_ActiveColorAttachment || passDepthAttachment != m_ActiveDepthAttachment)
		{
			graphicsContext.SetRenderTarget(*passColorAttachment, *passDepthAttachment);
			m_ActiveColorAttachment = passColorAttachment;
			m_ActiveDepthAttachment = passDepthAttachment;
		}
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
