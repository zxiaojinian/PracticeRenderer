#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	class Texture2D : public Texture
	{
	public:

		static std::shared_ptr<Texture2D> Create(const TextureSpecification& specification);
		static std::shared_ptr<Texture2D> Create(const TextureSpecification& specification, const std::string& path);

		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
	};
}

