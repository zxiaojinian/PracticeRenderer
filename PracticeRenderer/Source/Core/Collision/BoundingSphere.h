#pragma once

#include <glm/glm.hpp>

namespace PR
{
	struct BoundingSphere
	{
		glm::vec3 center{};
		float radius{ 0.0f };
	};
}
