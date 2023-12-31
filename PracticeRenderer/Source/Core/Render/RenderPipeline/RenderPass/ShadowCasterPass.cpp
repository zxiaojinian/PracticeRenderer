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

		m_MainLightShadowDataUBO = Buffer::Create(1, sizeof(MainLightShadowData), BufferType::UniformBuffer, BufferUsage::Dynamic);
		Shader::SetBuffer("MainLightShadowData", m_MainLightShadowDataUBO.get());
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
			SetupShadowCasterData(renderingData, m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex].radius * 2.0f);
			graphicsContext.SetRenderTarget(nullptr, m_Shadowmap.get(), 0, CubemapFace::Unknown, cascadeIndex, 0, CubemapFace::Unknown, cascadeIndex);
			graphicsContext.ClearRenderTarget(true, false, Color::clear);
			graphicsContext.SetViewProjectionMatrices(m_CascadeViewMatrices[cascadeIndex], m_CascadeProjectionMatrices[cascadeIndex]);
			DrawingSettings drawingSettings;
			drawingSettings.overrideMaterial = m_ShadowCasterMat.get();
			FilteringSettings filteringSettings;
			graphicsContext.DrawRenderers(renderingData.cullResults, drawingSettings, filteringSettings);
		}
		SetupMainLightShadowReceiverData(renderingData);
	}

	bool ShadowCasterPass::Setup(const RenderingData& renderingData)
	{
		uint32_t mainLightIndex = renderingData.mainLightIndex;
		if (mainLightIndex < 0)
			return false;

		auto light = renderingData.cullResults.VisibleLights[mainLightIndex];
		if (!light)
			return false;

		uint32_t shadowResolution = renderingData.shadowData.MainLightShadowmashadowResolution;
		uint32_t cascadesCount = renderingData.shadowData.MainLightCascadesCount;
		if (m_Shadowmap == nullptr || m_Shadowmap->GetWidth() != shadowResolution || m_Shadowmap->GetHeight() != shadowResolution)
		{
			RenderTextureSpecification specification = { shadowResolution, shadowResolution, cascadesCount, TextureFormat::D32_SFloat_S8_UInt,
				TextureWrapMode::Clamp, TextureFilterMode::Bilinear, false, TextureDimension::Tex2DArray };
			m_Shadowmap = RenderTexture::Create("Shadowmap", specification);
			m_Shadowmap->EnableCompare();
			Shader::SetTexture("MainLightShadowmap", m_Shadowmap.get());
		}

		InitShadowData();
		CalCascadesData(renderingData);
		return true;
	}

	void ShadowCasterPass::CalCascadesData(const RenderingData& renderingData)
	{
		float frustumIntervalBegin, frustumIntervalEnd;
		glm::vec3 frustumPointsInView[8];
		glm::vec3 lightCameraPos;
		glm::vec3 lightDir = renderingData.cullResults.VisibleLights[renderingData.mainLightIndex]->GetTransform().GetForward();
		glm::vec3 lightUp = renderingData.cullResults.VisibleLights[renderingData.mainLightIndex]->GetTransform().GetUp();

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
			CalFrustumBoundSphereInWorld(frustumPointsInView, m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex], renderingData.cameraData.camera->GetInvViewMatrix());

			BoundingSphere& cascadeShadowSplitSphere = m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex];
			float texelSize = cascadeShadowSplitSphere.radius * 2.0f / renderingData.shadowData.MainLightShadowmashadowResolution;
			//view matrix			
			cascadeShadowSplitSphere.center = renderingData.cullResults.VisibleLights[renderingData.mainLightIndex]->GetTransform().GetWorldToLocalMatrix() * glm::vec4(cascadeShadowSplitSphere.center, 1.0f);
			cascadeShadowSplitSphere.center.x = glm::round(cascadeShadowSplitSphere.center.x / texelSize) * texelSize;
			cascadeShadowSplitSphere.center.y = glm::round(cascadeShadowSplitSphere.center.y / texelSize) * texelSize;
			cascadeShadowSplitSphere.center = renderingData.cullResults.VisibleLights[renderingData.mainLightIndex]->GetTransform().GetLocalToWorldMatrix() * glm::vec4(cascadeShadowSplitSphere.center, 1.0f);
			lightCameraPos = cascadeShadowSplitSphere.center - lightDir * (cascadeShadowSplitSphere.radius + 30.0f);
			m_CascadeViewMatrices[cascadeIndex] = glm::lookAtRH(lightCameraPos, cascadeShadowSplitSphere.center, lightUp);

			//projection matrix
			float r = cascadeShadowSplitSphere.radius;
			m_CascadeProjectionMatrices[cascadeIndex] = glm::ortho(-r, r, -r, r, 0.0f, 2.0f * cascadeShadowSplitSphere.radius + 30.0f);//Temp near, far
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

	void ShadowCasterPass::SetupMainLightShadowReceiverData(const RenderingData& renderingData)
	{
		uint32_t cascadesCount = renderingData.shadowData.MainLightCascadesCount;
		for (uint32_t cascadeIndex = 0; cascadeIndex < cascadesCount; ++cascadeIndex)
		{
			m_MainLightShadowData.MainLightWorldToShadow[cascadeIndex] = m_CascadeProjectionMatrices[cascadeIndex] * m_CascadeViewMatrices[cascadeIndex];
			m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex].radius = m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex].radius * 
				m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex].radius;
		}

		m_MainLightShadowData.MainLightShadowmapSize = glm::vec4(static_cast<float>(renderingData.shadowData.MainLightShadowmashadowResolution), 1.0f / renderingData.shadowData.MainLightShadowmashadowResolution, 0.0f, 0.0f);
		m_MainLightShadowDataUBO->SetData(&m_MainLightShadowData, 0, 1, sizeof(MainLightShadowData));
	}

	glm::vec4 ShadowCasterPass::GetShadowBias(const ShadowData& shadowData, float frustumSize)
	{
		float texelSize = frustumSize / shadowData.MainLightShadowmashadowResolution;
		float lightDirectionBias = -shadowData.LightDirectionBias * texelSize;
		float normalBias = -shadowData.NormalBias * texelSize;
		return glm::vec4(lightDirectionBias, normalBias, 0.0f, 0.0f);
	}

	void ShadowCasterPass::SetupShadowCasterData(const RenderingData& renderingData, float frustumSize)
	{
		glm::vec4 bias = GetShadowBias(renderingData.shadowData, frustumSize);
		glm::vec4 lightDir = -glm::vec4(renderingData.cullResults.VisibleLights[renderingData.mainLightIndex]->GetTransform().GetForward(), 0.0f);
		Shader::SetFloat4("ShadowBias", bias);
		Shader::SetFloat4("LightDirection", lightDir);
	}

	void ShadowCasterPass::InitShadowData()
	{
		glm::mat4 zeroMatrix = glm::mat4(0.0f);
		for (uint32_t cascadeIndex = 0; cascadeIndex < MAX_CASCADES; ++cascadeIndex)
		{
			m_MainLightShadowData.MainLightWorldToShadow[cascadeIndex] = zeroMatrix;
			m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex].radius = 0.0f;
			m_MainLightShadowData.CascadeShadowSplitSpheres[cascadeIndex].center = glm::vec3(0.0f);
		}
		m_MainLightShadowData.MainLightWorldToShadow[MAX_CASCADES] = zeroMatrix;
	}
}
