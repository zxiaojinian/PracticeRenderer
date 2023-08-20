#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

namespace PR
{
	enum class RenderPassBlock : uint32_t
	{
		BeforeRendering = 0,
		MainRenderingOpaque,
		MainRenderingTransparent,
		AfterRendering,

		Count
	};

	class RenderBlocks
	{
	public:
		RenderBlocks(const std::vector<std::shared_ptr<RenderPass>>& activeRenderPassQueue);
		uint32_t GetLength(RenderPassBlock index) const;
		std::pair<uint32_t, uint32_t> GetRange(RenderPassBlock index) const;

	private:
		void FillBlockRanges(const std::vector<std::shared_ptr<RenderPass>>& activeRenderPassQueue);

	private:
		std::vector<RenderPassEvent> m_BlockEventLimits;
		std::vector<uint32_t> m_BlockRanges;
		std::vector<uint32_t> m_BlockRangeLengths;
	};
}

