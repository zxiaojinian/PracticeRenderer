#include "pch.h"
#include "BaseLight.h"

namespace PR
{
	BaseLight::BaseLight(GameObject& gameobject)
		: Component(gameobject)
	{
		m_DisallowMultipleComponent = true;
	}
}
