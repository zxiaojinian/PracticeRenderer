#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Collision/BoundingSphere.h"
#include "Core/Render/Buffer.h"

#include <glm/glm.hpp>

namespace PR
{
	#define MAX_CASCADES 4
	struct MainLightShadowData
	{
		glm::mat4 MainLightWorldToShadow[MAX_CASCADES + 1];
		glm::vec4 CascadeShadowSplitSpheres[MAX_CASCADES];
		float ShowCascade = 0.7f;
	};

	class ShadowCasterPass : public RenderPass
	{
	public:
		ShadowCasterPass(RenderPassEvent renderPassEvent);

		virtual void Configure() override;
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

		bool Setup(const RenderingData& renderingData);

	private:
		void CalCascadesData(const RenderingData& renderingData);
		void CalFrustumPointsInView(glm::vec3* points, const glm::mat4& invProjection, float nearPlane, float farPlane);
		void CalFrustumBoundSphereInWorld(glm::vec3* pointsInView, BoundingSphere& sphere, const glm::mat4& invViewMatrix);
		void SetupMainLightShadowReceiverConstants(const RenderingData& renderingData);
	private:
		std::shared_ptr<RenderTexture> m_Shadowmap;
		std::shared_ptr<Material> m_ShadowCasterMat;
		std::shared_ptr<Buffer> m_MainLightShadowDataUBO = nullptr;

		BoundingSphere m_CascadeBoundingSphere[MAX_CASCADES];
		glm::mat4 m_CascadeViewMatrices[MAX_CASCADES];
		glm::mat4 m_CascadeProjectionMatrices[MAX_CASCADES];
		glm::mat4 MainLightWorldToShadow[MAX_CASCADES + 1];

		float showCascade = 0.0f;
	};
}

