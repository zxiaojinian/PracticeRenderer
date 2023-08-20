#include "pch.h"
#include "ForwardRenderer.h"

namespace PR
{
	ForwardRenderer::ForwardRenderer()
	{
		m_DrawObjectsPass = std::make_shared<DrawObjectsPass>();
	}

	void ForwardRenderer::Setup(RenderingData& renderingData)
	{
		EnqueuePass(m_DrawObjectsPass);
	}
}
