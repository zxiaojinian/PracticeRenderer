#pragma once

enum class SortingCriteria
{
	None = 0 << 1,
	RenderQueue = 1 << 1,
	CommonOpaque = 2 << 1,
	CommonTransparent = 3 << 1,
};
