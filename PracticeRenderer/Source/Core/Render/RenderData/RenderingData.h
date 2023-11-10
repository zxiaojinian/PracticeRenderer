#pragma once

#include "CullingResults.h"
#include "CameraData.h"

namespace PR
{
	struct RenderingData
	{
		CullingResults cullResults;
		CameraData cameraData;
		int32_t mainLightIndex = -1;
	};
}
