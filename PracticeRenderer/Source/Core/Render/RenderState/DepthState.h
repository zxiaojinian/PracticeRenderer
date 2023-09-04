#pragma once

#include "CompareFunction.h"

struct DepthState
{
	bool writeEnabled = true;
	CompareFunction compareFunction = CompareFunction::Less;
};