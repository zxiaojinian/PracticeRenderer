#include "pch.h"
#include "FinalBlitPass.h"

#include <glad/glad.h>

namespace PR
{
	FinalBlitPass::FinalBlitPass(RenderPassEvent renderPassEvent, std::shared_ptr<Material>& blitMaterial) : RenderPass(renderPassEvent)
	{
		m_BlitMaterial = blitMaterial;
	}

	void FinalBlitPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		//draw fullscreen meshs

		glDisable(GL_DEPTH_TEST);

		graphicsContext.SetBackBuffer();
		graphicsContext.SetViewport(renderingData.cameraData.pixelRect);
		Shader::SetTexture("u_SourceTex", m_Source.get());
		graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_BlitMaterial);
	}

	void FinalBlitPass::Setup(std::shared_ptr<RenderTexture>& source)
	{
		m_Source = source;
	}
}
