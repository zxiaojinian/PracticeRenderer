#include "pch.h"
#include "ShadowCasterPass.h"

#include "Core/Asset/Resources.h"

namespace PR
{
	ShadowCasterPass::ShadowCasterPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		std::shared_ptr<Shader> shadowCasterShader = Resources::Get().LoadShader("Assets/Shader/ShadowCaster.shader");
		//Temp
		shadowCasterShader->GetRenderStateBlock().depthState.compareFunction = CompareFunction::LessEqual;
		shadowCasterShader->GetRenderStateBlock().depthState.writeEnabled = true;
		shadowCasterShader->GetRenderStateBlock().cullMode = CullMode::Back;
		shadowCasterShader->GetRenderStateBlock().blendState.WriteMask = ColorWriteMask::None;

		m_ShadowCasterMat = std::make_shared<Material>("ShadowCasterMat");
		m_ShadowCasterMat->SetShader(shadowCasterShader);
	}

	void ShadowCasterPass::Configure()
	{
		OverrideCameraTarget = true;
	}

	void ShadowCasterPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		uint32_t cascadesCount = renderingData.shadowData.MainLightCascadesCount;
		for (uint32_t cascadeIndex = 0; cascadeIndex < cascadesCount; ++cascadeIndex)
		{
			graphicsContext.SetRenderTarget(nullptr, m_Shadowmap.get(), 0, CubemapFace::Unknown, cascadeIndex, 0, CubemapFace::Unknown, cascadeIndex);
			graphicsContext.ClearRenderTarget(true, false, Color::clear);
			graphicsContext.SetViewProjectionMatrices(m_CascadeViewMatrices[cascadeIndex], m_CascadeProjectionMatrices[cascadeIndex]);
			DrawingSettings drawingSettings;
			drawingSettings.overrideMaterial = m_ShadowCasterMat.get();
			FilteringSettings filteringSettings;
			graphicsContext.DrawRenderers(renderingData.cullResults, drawingSettings, filteringSettings);
		}

		/*Shader::SetMat4("Matrix_MainLightShadow", m_ProjectionMatrix * m_ViewMatrix);*/
	}

	bool ShadowCasterPass::Setup(const RenderingData& renderingData)
	{
		uint32_t mainLightIndex = renderingData.mainLightIndex;
		if (mainLightIndex < 0)
			return false;

		auto light = renderingData.cullResults.VisibleLights[mainLightIndex];
		if (!light)
			return false;

		uint32_t shadowMapWidth = renderingData.shadowData.MainLightShadowmapWidth;
		uint32_t shadowMapHeight = renderingData.shadowData.MainLightShadowmapHeight;
		uint32_t cascadesCount = renderingData.shadowData.MainLightCascadesCount;
		if (m_Shadowmap == nullptr || m_Shadowmap->GetWidth() != shadowMapWidth || m_Shadowmap->GetHeight() != shadowMapHeight)
		{
			RenderTextureSpecification specification = { shadowMapWidth, shadowMapHeight, cascadesCount, TextureFormat::D32_SFloat_S8_UInt, 
				TextureWrapMode::Clamp, TextureFilterMode::Nearest, false, TextureDimension::Tex2DArray };
			m_Shadowmap = RenderTexture::Create("Shadowmap", specification);
			Shader::SetTexture("MainLightShadowmap", m_Shadowmap.get());
		}

		return true;
	}

	void ShadowCasterPass::CalCascadesData(const RenderingData& renderingData)
	{
		float frustumIntervalBegin, frustumIntervalEnd;
		glm::vec3 frustumPointsInView[8];
		glm::vec3 lightCameraPos;
		glm::vec3 lightDir = renderingData.cullResults.VisibleLights[renderingData.mainLightIndex]->GetTransform().GetForward();

		uint32_t cascadesCount = renderingData.shadowData.MainLightCascadesCount;
		auto cascadeCascadeSplit = renderingData.shadowData.MainLightShadowCascadesSplit;

		for (uint32_t cascadeIndex = 0; cascadeIndex < cascadesCount; ++cascadeIndex)
		{
			if (cascadeIndex == 0)
				frustumIntervalBegin = 0.0f;
			else
				frustumIntervalBegin = cascadeCascadeSplit[cascadeIndex - 1];
			frustumIntervalEnd = cascadeCascadeSplit[cascadeIndex];

			frustumIntervalBegin *= renderingData.shadowData.ShadowDistance;
			frustumIntervalEnd *= renderingData.shadowData.ShadowDistance;

			frustumIntervalBegin += renderingData.cameraData.camera->GetNearPlane();
			frustumIntervalEnd += renderingData.cameraData.camera->GetNearPlane();


			CalFrustumPointsInView(frustumPointsInView, renderingData.cameraData.camera->GetInvProjectionMatrix(), -frustumIntervalBegin, -frustumIntervalEnd);
			CalFrustumBoundSphereInWorld(frustumPointsInView, m_CascadeBoundingSphere[cascadeIndex], renderingData.cameraData.camera->GetInvViewMatrix());

			//view matrix
			lightCameraPos = m_CascadeBoundingSphere[cascadeIndex].center - lightDir * (m_CascadeBoundingSphere[cascadeIndex].radius + 10.0f);//Temp
			m_CascadeViewMatrices[cascadeIndex] = glm::lookAtRH(lightCameraPos, m_CascadeBoundingSphere[cascadeIndex].center, glm::vec3(0.0, 1.0f, 0.0f));

			//projection matrix
			float r = m_CascadeBoundingSphere[cascadeIndex].radius;
			m_CascadeProjectionMatrices[cascadeIndex] = glm::ortho(-r, r, r, r, 0.0f, 2.0f * m_CascadeBoundingSphere[cascadeIndex].radius + 10.0f);
		}
	}

	void ShadowCasterPass::CalFrustumPointsInView(glm::vec3* points, const glm::mat4& invProjection, float nearPlane, float farPlane)
	{
		//远裁剪面
		glm::vec4 frustumTL = invProjection * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
		frustumTL.x /= frustumTL.w;
		frustumTL.y /= frustumTL.w;
		frustumTL.z /= frustumTL.w;

		glm::vec4 frustumBR = invProjection * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
		frustumBR.x /= frustumBR.w;
		frustumBR.y /= frustumBR.w;
		frustumBR.z /= frustumBR.w;

		float factorMax = farPlane / frustumTL.z;
		glm::vec2 frustumTopLeftAtBack (frustumTL.x * factorMax, frustumTL.y * factorMax);//相似三角形
		glm::vec2 frustumBottomRightAtBack (frustumBR.x * factorMax, frustumBR.y * factorMax);

		float factorMin = nearPlane / frustumTL.z;
		glm::vec2 frustumTopLeftAtFront(frustumTL.x * factorMin, frustumTL.y * factorMin);
		glm::vec2 frustumBottomRightAtFront(frustumBR.x * factorMin, frustumBR.y * factorMin);

		points[0] = glm::vec3(frustumTopLeftAtFront.x, frustumTopLeftAtFront.y, nearPlane);
		points[1] = glm::vec3(frustumBottomRightAtFront.x, frustumTopLeftAtFront.y, nearPlane);
		points[2] = glm::vec3(frustumBottomRightAtFront.x, frustumBottomRightAtFront.y, nearPlane);
		points[3] = glm::vec3(frustumTopLeftAtFront.x, frustumBottomRightAtFront.y, nearPlane);

		points[4] = glm::vec3(frustumTopLeftAtBack.x, frustumTopLeftAtBack.y, farPlane);
		points[5] = glm::vec3(frustumBottomRightAtBack.x, frustumTopLeftAtBack.y, farPlane);
		points[6] = glm::vec3(frustumBottomRightAtBack.x, frustumBottomRightAtBack.y, farPlane);
		points[7] = glm::vec3(frustumTopLeftAtBack.x, frustumBottomRightAtBack.y, farPlane);
	}

	//https://zhuanlan.zhihu.com/p/568383387
	void ShadowCasterPass::CalFrustumBoundSphereInWorld(glm::vec3* pointsInView, BoundingSphere& sphere, const glm::mat4& invViewMatrix)
	{
		float len = std::abs(pointsInView[0].z - pointsInView[4].z);
		glm::vec3 a = pointsInView[0] - pointsInView[2];
		float a2 = glm::dot(a, a);

		glm::vec3 b = pointsInView[4] - pointsInView[6];
		float b2 = glm::dot(b, b);
		float x = len / 2.0f - (a2 - b2) / 8.0f / len;

		sphere.center = invViewMatrix * glm::vec4(0.0f, 0.0f, pointsInView[0].z - x, 1.0f);
		sphere.radius = std::sqrt(a2 / 4.0f + x * x);
	}
}
