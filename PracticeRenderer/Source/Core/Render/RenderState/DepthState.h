#pragma once

#include "CompareFunction.h"

namespace PR
{
	struct DepthState
	{
		bool writeEnabled = true;
		CompareFunction compareFunction = CompareFunction::LessEqual;
	};
}
