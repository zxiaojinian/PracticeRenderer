#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Render/RenderData/RenderingData.h"
#include "Core/Render/RenderPipeline/RenderBlocks.h"

namespace PR
{
	class Renderer
	{
	public:
		virtual void Setup(RenderingData& renderingData) = 0;
		virtual void SetupLights(const RenderingData& renderingData) {};
		virtual void FinishRendering(){}
		void Execute(const RenderingData& renderingData);
		void EnqueuePass(std::shared_ptr<RenderPass> renderPass);

	private:
		void ExecuteBlock(RenderPassBlock blockIndex, const RenderBlocks& renderBlocks, const RenderingData& renderingData);
		void ExecuteRenderPass(std::shared_ptr<RenderPass> renderPass, const RenderingData& renderingData);
		void SetRenderPassAttachments(std::shared_ptr<RenderPass> renderPass);

		void InternalStartRendering(const RenderingData& renderingData);
		void InternalFinishRendering();

	private:
		std::vector<std::shared_ptr<RenderPass>> m_ActiveRenderPassQueue;
	};
}


