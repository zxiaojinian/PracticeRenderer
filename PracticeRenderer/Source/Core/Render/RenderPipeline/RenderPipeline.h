#pragma once

#include "Core/Scene/Components/Camera.h"
#include "Core/Render/RenderData/CullingResults.h"

namespace PR
{
	class RenderPipeline
	{
	public:
		void Render();

	private:
		void RenderSingleCamera(Camera& camera);
		std::shared_ptr<CullingResults> Cull(Camera& camera);
		void SetupPerFrameShaderConstants();
		std::vector<Camera*> SortCameras(const std::vector<Camera*>& cameras);
	};
}


