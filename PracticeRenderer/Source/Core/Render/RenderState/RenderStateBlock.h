#pragma once

#include "DepthState.h"
#include "CullMode.h"

namespace PR
{
	struct RenderStateBlock
	{
		DepthState depthState{};
		CullMode cullMode{ CullMode::Back };
	};
}
