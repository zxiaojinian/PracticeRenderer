#pragma once

#include "DepthState.h"
#include "CullMode.h"
#include "BlendState.h"

namespace PR
{
	struct RenderStateBlock
	{
		DepthState depthState{};
		CullMode cullMode{ CullMode::Back };
		BlendState blendState{};
	};
}
