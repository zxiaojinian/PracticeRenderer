#include "pch.h"
#include "RenderBlocks.h"

namespace PR
{
	RenderBlocks::RenderBlocks(const std::vector<std::shared_ptr<RenderPass>>& activeRenderPassQueue)
		: m_BlockEventLimits(static_cast<uint32_t>(RenderPassBlock::Count)),
		m_BlockRanges(m_BlockEventLimits.size() + 1),
		m_BlockRangeLengths(m_BlockRanges.size())
	{
		m_BlockEventLimits[static_cast<uint32_t>(RenderPassBlock::BeforeRendering)] = RenderPassEvent::BeforeRenderingPrepasses;
		m_BlockEventLimits[static_cast<uint32_t>(RenderPassBlock::MainRenderingOpaque)] = RenderPassEvent::AfterRenderingOpaques;
		m_BlockEventLimits[static_cast<uint32_t>(RenderPassBlock::MainRenderingTransparent)] = RenderPassEvent::AfterRenderingPostProcessing;
		m_BlockEventLimits[static_cast<uint32_t>(RenderPassBlock::AfterRendering)] = (RenderPassEvent)0xFFFFFFFFU;

		FillBlockRanges(activeRenderPassQueue);

		for (size_t i = 0; i < m_BlockRanges.size() - 1; ++i)
		{
			m_BlockRangeLengths[i] = m_BlockRanges[i + 1] - m_BlockRanges[i];
		}
	}

	uint32_t RenderBlocks::GetLength(RenderPassBlock index) const
	{
		return m_BlockRangeLengths[static_cast<uint32_t>(index)];
	}

	std::pair<uint32_t, uint32_t> RenderBlocks::GetRange(RenderPassBlock index) const
	{
		return { m_BlockRanges[static_cast<uint32_t>(index)], m_BlockRanges[static_cast<uint32_t>(index) + 1] };
	}

	void RenderBlocks::FillBlockRanges(const std::vector<std::shared_ptr<RenderPass>>& activeRenderPassQueue)
	{
		uint32_t currRangeIndex = 0;
		uint32_t currRenderPass = 0;
		m_BlockRanges[currRangeIndex++] = 0;

		for (size_t i = 0; i < m_BlockEventLimits.size() - 1; ++i)
		{
			while (currRenderPass < activeRenderPassQueue.size() && activeRenderPassQueue[currRenderPass]->renderPassEvent < m_BlockEventLimits[i])
				currRenderPass++;

			m_BlockRanges[currRangeIndex++] = currRenderPass;
		}

		m_BlockRanges[currRangeIndex] = activeRenderPassQueue.size();
	}
}
