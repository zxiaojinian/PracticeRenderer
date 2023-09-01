#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Render/RenderData/RenderingData.h"
#include "Core/Render/RenderPipeline/RenderBlocks.h"
#include "Core/Render/GraphicsContext.h"

namespace PR
{
	class Renderer
	{
	public:
		virtual void Setup(RenderingData& renderingData) = 0;
		virtual void SetupLights(const RenderingData& renderingData) {};
		virtual void FinishRendering(){}
		void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData);
		void EnqueuePass(std::shared_ptr<RenderPass> renderPass);

		void ConfigureCameraTarget(std::shared_ptr<RenderTexture>& colorTarget, std::shared_ptr<RenderTexture>& depthTarget);

		void Clear();
	private:
		void ExecuteBlock(GraphicsContext& graphicsContext, RenderPassBlock blockIndex, const RenderBlocks& renderBlocks, const RenderingData& renderingData);
		void ExecuteRenderPass(GraphicsContext& graphicsContext, std::shared_ptr<RenderPass> renderPass, const RenderingData& renderingData);
		void SetCameraMatrices(const CameraData& cameraData);
		void SetRenderPassAttachments(GraphicsContext& graphicsContext, std::shared_ptr<RenderPass> renderPass);

		void InternalStartRendering(const RenderingData& renderingData);
		void InternalFinishRendering();

	private:
		std::vector<std::shared_ptr<RenderPass>> m_ActiveRenderPassQueue;

		std::shared_ptr<RenderTexture> m_CameraColorTarget = nullptr;
		std::shared_ptr<RenderTexture> m_CameraDepthTarget = nullptr;

		std::shared_ptr<RenderTexture> m_ActiveColorAttachment = nullptr;
		std::shared_ptr<RenderTexture> m_ActiveDepthAttachment = nullptr;
	};
}


