#pragma once

#include "Core/Scene/Components/Camera.h"

#include <glm/glm.hpp>

namespace PR
{
	class Renderer;

	struct CameraData
	{
		Camera* camera = nullptr;
		glm::vec4 pixelRect{ 0.0f, 0.0f, 1.0f, 1.0f };
		Renderer* render = nullptr;
	};
}

