#pragma once

#include "Core/Scene/Component.h"

#include<glm/glm.hpp>

namespace PR
{
	class Camera : public Component
	{
	public:
		Camera(GameObject& gameobject);
		glm::u32vec4 GetPixelRect() const;

		virtual std::string GetName() override { return "Camera"; }

	public:
		uint16_t RendererIndex = 0;
	};
}

