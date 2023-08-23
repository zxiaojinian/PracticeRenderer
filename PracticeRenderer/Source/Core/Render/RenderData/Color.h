#pragma once

#include <glm/glm.hpp>

namespace PR
{
	class Color
	{
	public:
		Color(float r, float g, float b, float a);
		Color(float r, float g, float b);

		float R, G, B, A;
		Color GetLinear() const;
		Color GetGamma() const;
		float Grayscale() const { return 0.299f * R + 0.587f * G + 0.114f * B; }

		static Color red;
		static Color green;
		static Color blue;
		static Color white;
		static Color black;
		static Color clear;
	};
}

