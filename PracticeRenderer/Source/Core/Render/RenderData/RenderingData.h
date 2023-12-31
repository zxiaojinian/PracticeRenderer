#pragma once

#include "CullingResults.h"

#include <glm/glm.hpp>

namespace PR
{
	struct ShadowData
	{
		uint32_t MainLightShadowmashadowResolution = 1024;
		uint32_t MainLightCascadesCount = 4;
		float ShadowDistance = 50.0f;
		float MainLightShadowCascadesSplit[4] = { 0.067f, 0.2f, 0.467f, 1.0f };
		float LightDirectionBias = 1.0f;
		float NormalBias = 1.0f;
	};

	class Renderer;

	struct CameraData
	{
		Camera* camera = nullptr;
		glm::u32vec4 pixelRect{ 0, 0, 1, 1 };
		Renderer* render = nullptr;
	};

	struct RenderingData
	{
		CullingResults cullResults;
		CameraData cameraData;
		int32_t mainLightIndex = -1;
		ShadowData shadowData;
	};
}
