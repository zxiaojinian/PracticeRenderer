#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(const TextureSpecification& specification);
		static std::shared_ptr<Texture2D> Create(const std::string& path);
	};
}

