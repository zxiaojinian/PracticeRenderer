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
		Cubemap,
		UBO,
		SSBO
	};

	struct PropertyData
	{
		PropertyType Type;
		std::string Name;
		bool IsImage = false;
	};

	struct PropertyValue
	{
		PropertyType Type;
		std::any Value;
		uint32_t MipLevel = 0;
	};
}
