#include "pch.h"
#include "ForwardRenderer.h"

namespace PR
{
	ForwardRenderer::ForwardRenderer()
	{
		std::shared_ptr<Shader> shader = Shader::Create("Assets/Blit.glsl");
		m_BlitMaterial = std::make_shared<Material>("BlitMaterial");
		m_BlitMaterial->SetShader(shader);

		m_DrawObjectsPass = std::make_shared<DrawObjectsPass>(RenderPassEvent::AfterRenderingShadows);
		m_FinalBlitPass = std::make_shared<FinalBlitPass>(RenderPassEvent::AfterRendering, m_BlitMaterial);
	}

	void ForwardRenderer::Setup(RenderingData& renderingData)
	{
		EnqueuePass(m_DrawObjectsPass);
		EnqueuePass(m_FinalBlitPass);
	}
}
