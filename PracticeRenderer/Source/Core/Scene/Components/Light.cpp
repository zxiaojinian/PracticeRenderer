#include "pch.h"
#include "Light.h"

#include "Core/Scene/Components/Transform.h"

namespace PR
{
	Light::Light(GameObject& gameobject)
		: Component(gameobject)
	{
		m_DisallowMultipleComponent = true;
	}

	void Light::UpdateLightData()
	{
		if (LightType == LightType::Directional)
		{
			glm::vec3 dir = m_Transform.GetForward();
			m_LightData.Position = { dir, 0.0f };
		}
		else
		{
			glm::vec3 pos = m_Transform.GetPosition();
			m_LightData.Position = { pos, 1.0f };
		}
		m_LightData.Color = Color;
	}

	bool Light::TestVisible()
	{
		
	}
}
