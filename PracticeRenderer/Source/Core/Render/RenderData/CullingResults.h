#pragma once

#include "Core/Scene/Components/Light.h"
#include "Core/Scene/GameObject.h"

#include<vector>

namespace PR
{
	struct CullingResults
	{
		std::vector<Light*> VisibleLights;
		std::vector<GameObject*> VisibleGameobjects;
	};
}
