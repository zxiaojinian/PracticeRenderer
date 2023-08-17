#pragma once

#include<vector>

namespace PR
{
	struct CullingResults
	{
		std::vector<uint32_t> VisibleLights;
		std::vector<uint32_t> VisibleGameobjects;
	};
}
