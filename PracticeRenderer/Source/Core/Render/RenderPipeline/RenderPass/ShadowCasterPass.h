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
//PCF--存在2个问题
	//1，PCF样本可能在该级联范围外，如果是纹理数组，设置寻找方式为clamp应该没问题。如果是图集，需要为该级联外扩一半pcf范围
	//2，PCF样本范围过大，会加剧Shadow Acne，因为和阴影纹理样本比较不是其对应几何物体的光空间深度。
namespace PR
{
	#define MAX_CASCADES 4
	struct MainLightShadowData
	{
		glm::mat4 MainLightWorldToShadow[MAX_CASCADES + 1];
		BoundingSphere CascadeShadowSplitSpheres[MAX_CASCADES];
		glm::vec4 MainLightShadowmapSize{};
		float ShowCascade = 0.0f;
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
		void InitShadowData();
	private:
		std::shared_ptr<RenderTexture> m_Shadowmap;
		std::shared_ptr<Material> m_ShadowCasterMat;
		std::shared_ptr<Buffer> m_MainLightShadowDataUBO = nullptr;

		glm::mat4 m_CascadeViewMatrices[MAX_CASCADES];
		glm::mat4 m_CascadeProjectionMatrices[MAX_CASCADES];

		MainLightShadowData m_MainLightShadowData;
	};
}

