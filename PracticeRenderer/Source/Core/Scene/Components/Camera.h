#pragma once

#include "Core/Scene/Component.h"

namespace PR
{
	class Camera : public Component
	{
	public:
		Camera(GameObject& gameobject);

		virtual std::string GetName() override { return "Camera"; }
	};
}

