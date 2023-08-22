#pragma once

#include "CullingResults.h"
#include "CameraData.h"
#include "LightData.h"

namespace PR
{
	struct RenderingData
	{
		CullingResults cullResults;
		CameraData cameraData;
		LightData lightData;
	};
}
