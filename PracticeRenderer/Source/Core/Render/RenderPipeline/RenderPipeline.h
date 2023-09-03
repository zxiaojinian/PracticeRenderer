#pragma once

#include "Core/Scene/Components/Camera.h"
#include "Core/Render/RenderData/CullingResults.h"
#include "Core/Render/RenderPipeline/Renderer.h"
#include "Core/Render/RenderData/RenderingData.h"
#include "Core/Render/RenderData/CameraData.h"
#include "Core/Render/GraphicsContext.h"

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

		void Render(GraphicsContext& graphicsContext);

	private:
		void RenderSingleCamera(GraphicsContext& graphicsContext, CameraData& cameraData);
		std::shared_ptr<CullingResults> Cull(Camera& camera);
		void SetupPerFrameShaderConstants(GraphicsContext& graphicsContext);
		std::vector<Camera*> SortCameras(const std::vector<Camera*>& cameras);

		void CreateDefaultRenderer();
		Renderer* GetRenderer(uint16_t index);
		void InitializeCameraData(Camera* camera, CameraData& cameraData);
		void InitializeRenderingData(RenderingData& renderingData, CameraData& cameraData);

	private:
		std::vector<std::shared_ptr<Renderer>> m_Renderers;
		RenderPath m_RenderPath = RenderPath::Forward;
	};
}


