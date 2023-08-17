#include "pch.h"
#include "RenderPipeline.h"

#include "Core/Render/RenderCommand.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Components/MeshRenderer.h"

namespace PR
{
	void RenderPipeline::Render()
	{
		RenderCommand::SetClearColor({ 0.5f, 0.5f, 0.5f,0.5f });
		RenderCommand::Clear();

		auto& goes = SceneManager::Get().GetCurrentScene()->GetGameObjecs();
		for (auto g : goes)
		{
			auto mesh = g->GetComponent<MeshRenderer>()->GetMesh();
			mesh->Bind();
			RenderCommand::DrawIndexed(nullptr, mesh->GetIndexCount());
		}
	}
}
