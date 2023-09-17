#include "pch.h"
#include "Renderer.h"

namespace PR
{
	Renderer::Renderer()
	{
		m_CameraMatrixUBO = Buffer::Create(1, sizeof(glm::mat4) * 3, BufferType::UniformBuffer, BufferUsage::Dynamic);
		Shader::SetBuffer("CameraMatrix", m_CameraMatrixUBO.get());

		m_CameraDataUBO = Buffer::Create(1, sizeof(glm::vec4), BufferType::UniformBuffer, BufferUsage::Dynamic);
		Shader::SetBuffer("CameraData", m_CameraDataUBO.get());
	}

	void Renderer::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		std::sort(m_ActiveRenderPassQueue.begin(), m_ActiveRenderPassQueue.end(), [](const std::shared_ptr<RenderPass>& l, const std::shared_ptr<RenderPass>& r){
				return *l < *r;
			});
		InternalStartRendering(renderingData);
		SetPerCameraShaderVariables(renderingData.cameraData);
		SetupLights(graphicsContext, renderingData);

		RenderBlocks renderBlocks(m_ActiveRenderPassQueue);
		if(renderBlocks.GetLength(RenderPassBlock::BeforeRendering))
			ExecuteBlock(graphicsContext, RenderPassBlock::BeforeRendering, renderBlocks, renderingData);

		SetCameraMatrices(renderingData.cameraData);
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

	void Renderer::SetCameraMatrices(const CameraData& cameraData)
	{
		auto& vMatrix = cameraData.camera->GetViewMatrix();
		auto& pMatrix = cameraData.camera->GetProjectionMatrix();
		auto& vpMatrix = cameraData.camera->GetViewProjectionMatrix();
		glm::mat4 cameraMatrices[] = {vMatrix , pMatrix, vpMatrix};
		m_CameraMatrixUBO->SetData(cameraMatrices, 0, 1, sizeof(glm::mat4) * 3);
	}

	void Renderer::SetPerCameraShaderVariables(const CameraData& cameraData)
	{
		if (cameraData.camera)
		{
			glm::vec4 cameraPosWS = glm::vec4(cameraData.camera->GetTransform().GetPosition(), 1.0f);
			m_CameraDataUBO->SetData(&cameraPosWS, 0, 1, sizeof(glm::vec4));
		}
	}

	void Renderer::ConfigureCameraTarget(std::shared_ptr<RenderTexture>& colorTarget, std::shared_ptr<RenderTexture>& depthTarget)
	{
		m_CameraColorTarget = colorTarget;
		m_CameraDepthTarget = depthTarget;
	}

	void Renderer::Clear()
	{
		m_CameraColorTarget = nullptr;
		m_CameraDepthTarget = nullptr;

		m_ActiveColorAttachment = nullptr;
		m_ActiveDepthAttachment = nullptr;
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
			graphicsContext.ClearRenderTarget(true, true, Color::clear);
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
