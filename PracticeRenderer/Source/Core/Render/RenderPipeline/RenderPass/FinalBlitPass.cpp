#include "pch.h"
#include "FinalBlitPass.h"

namespace PR
{
	FinalBlitPass::FinalBlitPass(RenderPassEvent renderPassEvent, std::shared_ptr<Material>& blitMaterial) : RenderPass(renderPassEvent)
	{
		m_BlitMaterial = blitMaterial;
	}

	void FinalBlitPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		//draw fullscreen meshs
		graphicsContext.SetBackBuffer();
		Shader::SetTexture("u_SourceTex", m_Source.get());
		graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_BlitMaterial);
	}

	void FinalBlitPass::Setup(std::shared_ptr<RenderTexture>& source)
	{
		m_Source = source;
	}
}
