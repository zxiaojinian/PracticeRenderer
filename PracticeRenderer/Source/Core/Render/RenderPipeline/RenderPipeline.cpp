#include "pch.h"
#include "RenderPipeline.h"

#include "Core/Render/RenderCommand.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Components/MeshRenderer.h"

namespace PR
{
	void RenderPipeline::Render()
	{
		SetupPerFrameShaderConstants();

		Scene* scene = SceneManager::Get().GetCurrentScene();
		auto& cameras = SortCameras(scene->GetCameras());
		for (auto camera : cameras)
		{
			if (camera)
			{
				RenderSingleCamera(*camera);
			}
				
		}
	}

	void RenderPipeline::RenderSingleCamera(Camera& camera)
	{
		RenderCommand::SetClearColor({ 0.5f, 0.5f, 0.5f,0.5f });
		RenderCommand::Clear();

		auto cullResults = Cull(camera);

		auto& renders = SceneManager::Get().GetCurrentScene()->GetMeshRenderers();
		for (auto render : renders)
		{
			auto mesh = render->GetMesh();
			mesh->Bind();
			RenderCommand::DrawIndexed(nullptr, mesh->GetIndexCount());
		}
	}

	std::shared_ptr<CullingResults> RenderPipeline::Cull(Camera& camera)
	{
		auto cullResults = std::shared_ptr<CullingResults>();
		return cullResults;
	}

	void RenderPipeline::SetupPerFrameShaderConstants()
	{
	}
	std::vector<Camera*> RenderPipeline::SortCameras(const std::vector<Camera*>& cameras)
	{
		return std::vector<Camera*>(cameras);
	}
}
