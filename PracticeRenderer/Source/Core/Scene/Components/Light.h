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

		const LightData& GetLightData() { return m_LightData; }
		void UpdateLightData();
		bool TestVisible();

	public:
		LightType LightType = LightType::Directional;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
		ShadowType ShadowType = ShadowType::SoftShadow;
		float ShadowStrength = 1.0f;

	private:
		LightData m_LightData;
	};
}

