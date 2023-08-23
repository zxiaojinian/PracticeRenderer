#include "pch.h"
#include "Color.h"

#include <glm/gtc/color_space.hpp>

namespace PR
{
	Color Color::red{ 1.0f, 0.0f, 0.0f, 1.0f };
	Color Color::green{ 0.0f, 1.0f, 0.0f, 1.0f };
	Color Color::blue{ 0.0f, 0.0f, 1.0f, 1.0f };
	Color Color::white{ 1.0f, 1.0f, 1.0f, 1.0f };
	Color Color::black{ 0.0f, 0.0f, 0.0f, 1.0f };
	Color Color::clear{ 0.0f, 1.0f, 0.0f, 0.0f };

	Color::Color(float r, float g, float b, float a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	Color::Color(float r, float g, float b)
	{
		R = r;
		G = g;
		B = b;
		A = 1.0f;
	}

	Color Color::GetLinear() const
	{
		glm::vec3 color = { R, G, B };
		auto linearColor = glm::convertSRGBToLinear(color);
		return { linearColor.r, linearColor.g, linearColor.b, A };
	}

	Color Color::GetGamma() const
	{
		glm::vec3 color = { R, G, B };
		auto srgbColor = glm::convertLinearToSRGB(color);
		return { srgbColor.r, srgbColor.g, srgbColor.b, A };
	}
}
