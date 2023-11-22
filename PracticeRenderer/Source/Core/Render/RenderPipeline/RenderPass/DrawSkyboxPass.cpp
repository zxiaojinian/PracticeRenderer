#include "pch.h"
#include "DrawSkyboxPass.h"

#include "Core/Asset/Resources.h"

namespace PR
{
	DrawSkyboxPass::DrawSkyboxPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		std::shared_ptr<Shader> skyBox = Resources::Get().LoadShader("Assets/Shader/SkyBox.shader");
		//Temp
		skyBox->GetRenderStateBlock().depthState.compareFunction = CompareFunction::LessEqual;
		skyBox->GetRenderStateBlock().depthState.writeEnabled = false;
		skyBox->GetRenderStateBlock().cullMode = CullMode::Disabled;
		m_SkyBoxMaterial = std::make_shared<Material>("SkyBoxMat");
		m_SkyBoxMaterial->SetShader(skyBox);

		std::vector<std::string> facesPath
		{
			"Assets/Texture/SkyBox/skybox_posx.hdr",
			"Assets/Texture/SkyBox/skybox_negx.hdr",
			"Assets/Texture/SkyBox/skybox_posy.hdr",
			"Assets/Texture/SkyBox/skybox_negy.hdr",
			"Assets/Texture/SkyBox/skybox_posz.hdr",
			"Assets/Texture/SkyBox/skybox_negz.hdr",
		};
		skyCubeMap = Resources::Get().LoadCubemap("SkyBox", facesPath, TextureFilterMode::Trilinear, TextureWrapMode::Clamp, true);
		skyCubeMap->SetSmoothEdges(true);
		m_SkyBoxMaterial->SetCubemap("SkyCubeMap", skyCubeMap.get());

		auto meshes = Resources::Get().LoadMeshes("Assets/Model/SkyBoxSphere.obj");
		if (meshes.size() > 0)
		{
			m_SkyMesh = meshes[0];
		}
	}

	void DrawSkyboxPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		if (m_SkyMesh && m_SkyBoxMaterial)
		{
			glm::vec3 cameraPosWS = renderingData.cameraData.camera->GetTransform().GetPosition();
			glm::vec3 scale = glm::vec3(renderingData.cameraData.camera->GetFarPlane() * 0.99f);
			glm::mat4 skyBoxMatrix = glm::translate(glm::mat4(1.0f), cameraPosWS) * glm::scale(glm::mat4(1.0f), scale);
			graphicsContext.DrawMesh(*m_SkyMesh, skyBoxMatrix, *m_SkyBoxMaterial);
		}
	}
}
