#pragma once

#include "CompareFunction.h"

struct DepthState
{
	bool writeEnabled = true;
	CompareFunction compareFunction = CompareFunction::LessEqual;

	DepthState(bool enableWrite, CompareFunction compareFunction)
		: writeEnabled(enableWrite), compareFunction(compareFunction)
	{}
};