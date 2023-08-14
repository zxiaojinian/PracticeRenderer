#pragma once

#include "Core/Scene/Component.h"
#include "Core/Render/RenderData/LightData.h"

#include <glm/glm.hpp>

namespace PR
{
	enum  class LightType
	{
		Spot,
		Directional,
		Point
	};

	enum class ShadowType
	{
		NoShadow,
		HardShadow,
		SoftShadow
	};

	class Light : public Component
	{
		
	public:
		Light(GameObject& gameobject);

		const LightData& GetLightData();
		void UpdateLightData();
		bool TestVisible();
		void GetLightAttenuationAndSpotDirection();
		virtual std::string GetName() override { return "Light"; }

	public:
		LightType Type = LightType::Directional;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
		float LightRange = 10.0f;
		float SpotAngle = 30.0f;
		float InnerSpotAngle = 21.0f;
		ShadowType ShadowType = ShadowType::SoftShadow;
		float ShadowStrength = 1.0f;

	private:
		LightData m_LightData;

		static glm::vec4 s_DefaultLightAttenuation;
		static glm::vec4 s_DefaultLightSpotDirection;
	};
}

