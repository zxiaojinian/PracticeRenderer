#pragma once

#include "Core/Render/Texture.h"

namespace PR
{
	class TextureCube : Texture
	{
		static std::shared_ptr<TextureCube> Create(const TextureSpecification& specification);
	};
}


