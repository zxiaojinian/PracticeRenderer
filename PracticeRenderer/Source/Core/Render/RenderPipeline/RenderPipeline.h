#pragma once

#include "Core/Scene/Components/Camera.h"
#include "Core/Render/RenderData/CullingResults.h"
#include "Core/Render/RenderPipeline/Renderer.h"
#include "Core/Render/RenderData/RenderingData.h"

namespace PR
{
	enum class RenderPath
	{
		Forward,
		Deferred
	};

	class RenderPipeline
	{
	public:
		RenderPipeline();
		void SetRenderPath(RenderPath renderPath);

		void Render();

	private:
		void RenderSingleCamera(Camera& camera);
		std::shared_ptr<CullingResults> Cull(Camera& camera);
		void SetupPerFrameShaderConstants();
		std::vector<Camera*> SortCameras(const std::vector<Camera*>& cameras);

		void CreateDefaultRenderer();
		std::shared_ptr<Renderer> GetRenderer(uint16_t index);
		void InitializeRenderingData(RenderingData& renderingData);

	private:
		std::vector<std::shared_ptr<Renderer>> m_Renderers;
		RenderPath m_RenderPath = RenderPath::Forward;
	};
}


