#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Collision/BoundingSphere.h"

#include <glm/glm.hpp>

namespace PR
{
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
	private:
		std::shared_ptr<RenderTexture> m_Shadowmap;
		std::shared_ptr<Material> m_ShadowCasterMat;

		#define MAX_CASCADES 4
		BoundingSphere m_CascadeBoundingSphere[MAX_CASCADES];
		glm::mat4 m_CascadeViewMatrices[MAX_CASCADES];
		glm::mat4 m_CascadeProjectionMatrices[MAX_CASCADES];
		glm::mat4 m_CascadeMatrices[MAX_CASCADES];
	};
}

