#include "pch.h"
#include "PostProcessPass.h"

namespace PR
{
	PostProcessPass::PostProcessPass(RenderPassEvent renderPassEvent, std::shared_ptr<Material>& blitMaterial) : RenderPass(renderPassEvent)
	{
		m_BlitMaterial = blitMaterial;
	}

	void PostProcessPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		//draw fullscreen meshs
		graphicsContext.SetBackBuffer();
		graphicsContext.SetViewport(renderingData.cameraData.pixelRect);
		Shader::SetTexture("u_SourceTex", m_Source.get());
		graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_BlitMaterial);
	}

	void PostProcessPass::Setup(std::shared_ptr<RenderTexture>& source)
	{
		m_Source = source;
	}
}
