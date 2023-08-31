#pragma once

#include "Core/Scene/Components/Camera.h"

#include <glm/glm.hpp>

namespace PR
{
	class Renderer;

	struct CameraData
	{
		Camera* camera = nullptr;
		glm::u32vec4 pixelRect{ 0, 0, 1, 1 };
		Renderer* render = nullptr;
	};
}

