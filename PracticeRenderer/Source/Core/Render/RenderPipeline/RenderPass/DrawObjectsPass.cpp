#include "pch.h"
#include "DrawObjectsPass.h"

#include "Core/Render/RenderCommand.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Components/MeshRenderer.h"

namespace PR
{
	void DrawObjectsPass::Execute(const RenderingData& renderingData)
	{
		RenderCommand::SetClearColor({ 0.5f, 0.5f, 0.5f,0.5f });
		RenderCommand::Clear();
		auto& renders = SceneManager::Get().GetCurrentScene()->GetMeshRenderers();
		for (auto render : renders)
		{
			auto mesh = render->GetMesh();
			mesh->Bind();
			auto& mats = render->GetMaterials();
			for(auto& mat : mats)
			{
				mat->UploadProperty();
			}
			RenderCommand::DrawIndexed(nullptr, mesh->GetIndexCount());
		}
	}
}
