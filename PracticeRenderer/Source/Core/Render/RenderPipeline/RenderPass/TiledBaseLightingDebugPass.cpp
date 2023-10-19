#include "pch.h"
#include "TiledBaseLightingDebugPass.h"
#include "Core/Asset/Resources.h"

namespace PR
{
	TiledBaseLightingDebugPass::TiledBaseLightingDebugPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		std::shared_ptr<Shader> shader = Resources::Get().LoadShader("Assets/Shader/LightCulling/TiledCulling/TiledCullingDebug.shader");
		//Temp
		shader->GetRenderStateBlock().depthState.compareFunction = CompareFunction::Disabled;
		shader->GetRenderStateBlock().depthState.writeEnabled = false;
		shader->GetRenderStateBlock().cullMode = CullMode::Disabled;
		m_DebugMat = std::make_shared<Material>("TiledCullingDebugMat");
		m_DebugMat->SetShader(shader);
	}

	void TiledBaseLightingDebugPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_DebugMat);
	}
}
