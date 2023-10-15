#pragma once

#include "SortingCriteria.h"
#include "Core/Render/Asset/Material.h"

namespace PR
{
	struct DrawingSettings
	{
		SortingCriteria sortingCriteria{ SortingCriteria::CommonOpaque };
		Material* overrideMaterial = nullptr;
	};
}


