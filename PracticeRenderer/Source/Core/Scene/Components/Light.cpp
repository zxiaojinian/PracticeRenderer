#include "pch.h"
#include "Light.h"

#include "Core/Scene/Components/Transform.h"

namespace PR
{
	glm::vec4 Light::s_DefaultLightAttenuation = { 0.0f, 1.0f, 0.0f, 1.0f };
	glm::vec4 Light::s_DefaultLightSpotDirection = { 0.0f, 0.0f, 1.0f, 0.0f };

	Light::Light(GameObject& gameobject)
		: Component(gameobject)
	{
		m_DisallowMultipleComponent = true;
	}

	const LightData& Light::GetLightData()
	{
		UpdateLightData(); 
		return m_LightData;
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
		GetLightAttenuationAndSpotDirection();
	}

	bool Light::TestVisible()
	{
		//TODO Frustum Test Sphere
		return true;
	}

	void Light::GetLightAttenuationAndSpotDirection()
	{
		m_LightData.Attenuation = s_DefaultLightAttenuation;
		m_LightData.SpotDirection = s_DefaultLightSpotDirection;

		if (LightType != LightType::Directional)
		{
			//From URP
			//attenuation = smoothFactor * 1.0 / distanceToLightSqr
			//smoothFactor = (1.0 - (distanceToLightSqr * 1.0 / lightRangeSqr) ^ 2) ^ 2
			float oneOverLightRangeSqr = 1.0f / std::max(0.0001f, LightRange * LightRange);			
			m_LightData.Attenuation.x = oneOverLightRangeSqr;
		}

		if (LightType == LightType::Spot)
		{
			glm::vec3 dir = m_Transform.GetForward();
			m_LightData.SpotDirection = { dir, 0.0f };

			//Form URP
			// Spot Attenuation with a linear falloff can be defined as
			// (SdotL - cosOuterAngle) / (cosInnerAngle - cosOuterAngle)
			// This can be rewritten as
			// invAngleRange = 1.0 / (cosInnerAngle - cosOuterAngle)
			// SdotL * invAngleRange + (-cosOuterAngle * invAngleRange)
			// If we precompute the terms in a MAD instruction
			float cosOuterAngle = std::cos(glm::radians(SpotAngle * 0.5f));
			float cosInnerAngle = std::cos(glm::radians(InnerSpotAngle * 0.5f));
			float smoothAngleRange = std::max(0.001f, cosInnerAngle - cosOuterAngle);
			float invAngleRange = 1.0f / smoothAngleRange;
			float add = -cosOuterAngle * invAngleRange;
			m_LightData.Attenuation.z = invAngleRange;
			m_LightData.Attenuation.w = add;
		}
	}
}
