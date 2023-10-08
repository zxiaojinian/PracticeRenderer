#pragma once

#include <string>
#include <any>

namespace PR
{
	enum class PropertyType
	{
		Unknown,
		Int,
		Float,
		Float2,
		Float3,
		Float4,
		Mat4,
		Texture,
		UBO,
		SSBO
	};

	struct PropertyData
	{
		PropertyType Type;
		std::string Name;
	};

	struct PropertyValue
	{
		PropertyType Type;
		std::any Value;
	};
}
