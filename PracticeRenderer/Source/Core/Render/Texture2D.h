#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static std::shared_ptr<Texture2D> Create(const std::string& path);
	};
}

