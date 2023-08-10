#pragma once

#include "Core/Scene/Component.h"

namespace PR
{
	class BaseLight : public Component
	{
		
	public:
		BaseLight(GameObject& gameobject);
		~BaseLight() = default;

	public:
	};
}

