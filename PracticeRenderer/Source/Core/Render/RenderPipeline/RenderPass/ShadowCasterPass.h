#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Collision/BoundingSphere.h"
#include "Core/Render/Buffer.h"

#include <glm/glm.hpp>

//Projective Aliasing --csm
//Shadow Acne--light direction depth bias + normal direction bias
//Shadow swimming.--根本原因是场景像素帧与帧之间采样到阴影纹理深度值发生变化。
	//1.旋转相机,阴影视锥体大小发生变化，导致阴影纹理纹素覆盖的世界空间范围发生变化，需要固定阴影视锥体大小。
	//2.移动相机，如果2帧之间，阴影纹理纹素中心移动距离不是其覆盖的世界空间范围的整数倍就会发生闪烁，需要以阴影纹理的纹素覆盖的世界空间范围来吸附阴影视锥体中心位置
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
		void SetupMainLightShadowReceiverData(const RenderingData& renderingData);
		glm::vec4 GetShadowBias(const ShadowData& shadowData, float frustumSize);
		void SetupShadowCasterData(const RenderingData& renderingData, float frustumSize);
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

