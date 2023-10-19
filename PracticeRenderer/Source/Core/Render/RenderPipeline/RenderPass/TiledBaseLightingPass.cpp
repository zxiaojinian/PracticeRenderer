#include "pch.h"
#include "TiledBaseLightingPass.h"

#include <glm/gtc/matrix_inverse.hpp>

namespace PR
{
	TiledBaseLightingPass::TiledBaseLightingPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		m_DepthBoundsCS = ComputeShader::Create("Assets/Shader/LightCulling/TiledCulling/DepthBounds.compute");
		m_LightCullingCS = ComputeShader::Create("Assets/Shader/LightCulling/TiledCulling/TiledLightCulling.compute");
	}

	void TiledBaseLightingPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		CameraData cameraData = renderingData.cameraData;
		if (m_PixelWidth != cameraData.pixelRect.z || m_pixelHeight != cameraData.pixelRect.w)
		{
			const float INV_TILE_SIZE = 1.0f / TILE_SIZE;
			m_PixelWidth = cameraData.pixelRect.z;
			m_pixelHeight = cameraData.pixelRect.w;
			m_ScreenParams = glm::vec4(m_PixelWidth, m_pixelHeight, 1.0f / m_PixelWidth, 1.0f / m_pixelHeight);
			m_TileCountX = static_cast<uint32_t>(ceil(m_PixelWidth * INV_TILE_SIZE));
			m_TileCountY = static_cast<uint32_t>(ceil(m_pixelHeight * INV_TILE_SIZE));
			m_TileCount = m_TileCountX * m_TileCountY;

			ComputeShader::SetGlobalFloat4("ScreenParams_CS", m_ScreenParams);
		}

		//DepthBounds
		if (m_DepthBounds == nullptr || m_DepthBounds->GetWidth() != m_PixelWidth || m_DepthBounds->GetHeight() != m_pixelHeight)
		{
			Texture2DSpecification specification{};
			specification.Width = m_TileCountX;
			specification.Height = m_TileCountY;
			specification.Format = TextureFormat::R32G32B32A32_SFloat;
			specification.WrapMode = TextureWrapMode::Clamp;
			specification.FilterMode = TextureFilterMode::Nearest;
			specification.GenerateMips = false;

			m_DepthBounds = Texture2D::Create("depthBounds", specification);

			ComputeShader::SetGlobalTexture("depthBounds", m_DepthBounds.get());
		}

		if (cameraData.camera && m_PreProjectionMatrix != cameraData.camera->GetProjectionMatrix())
		{
			m_PreProjectionMatrix = cameraData.camera->GetProjectionMatrix();
			m_InvProjectionMatrix = glm::inverse(m_PreProjectionMatrix);

			ComputeShader::SetGlobalMat4("InverseProjection", m_InvProjectionMatrix);
		}

		//m_DepthBoundsCS->SetFloat4("ScreenParams_CS", m_ScreenParams);
		//m_DepthBoundsCS->SetMat4("InverseProjection", m_InvProjectionMatrix);
		//m_DepthBoundsCS->SetTexture("depthBounds", m_DepthBounds.get());
		graphicsContext.DispatchCompute(*m_DepthBoundsCS, m_TileCountX, m_TileCountY, 1);

		//light culling
		auto& visibleLights = renderingData.cullResults.VisibleLights;
		if (m_LightCullingDataBuffer == nullptr || m_LightCullingDataBuffer->GetCount() != visibleLights.size())
		{
			m_LightCullingDataBuffer = Buffer::Create(static_cast<uint32_t>(visibleLights.size()), sizeof(LightData), BufferType::StorageBuffer, BufferUsage::Dynamic);
		}

		std::vector<glm::vec4> lightsCullingData;
		for (auto light : visibleLights)
		{
			if(light && cameraData.camera)
				lightsCullingData.push_back(GetLightSphereData(*light, *cameraData.camera));
		}
		auto stride = static_cast<uint32_t>(sizeof(glm::vec4));
		m_LightCullingDataBuffer->SetData(lightsCullingData.data(), 0, static_cast<uint32_t>(lightsCullingData.size()), stride);

		uint32_t lightIndexListCountDouble = (2 * MAX_NUM_LIGHTS_PER_TILE + 4) * m_TileCount;
		if (m_LightIndexListDoubleBuffer == nullptr || m_LightIndexListDoubleBuffer->GetCount() != lightIndexListCountDouble)
		{
			m_LightIndexListDoubleBuffer = Buffer::Create(lightIndexListCountDouble, sizeof(uint32_t), BufferType::StorageBuffer, BufferUsage::Dynamic);
		}

		//m_LightCullingCS->SetTexture("m_LightCullingCS", m_DepthBounds.get());
		m_LightCullingCS->SetBuffer("LightsCullingDataBuffer", m_LightCullingDataBuffer.get());
		m_LightCullingCS->SetBuffer("LightIndexListDoubleBuffer", m_LightIndexListDoubleBuffer.get());
		graphicsContext.DispatchCompute(*m_LightCullingCS, m_TileCountX, m_TileCountY, 1);

		Shader::SetBuffer("LightIndexListDoubleBuffer", m_LightIndexListDoubleBuffer.get());
		Shader::SetInt("NumTilesX", m_TileCountX);
	}

	glm::vec4 TiledBaseLightingPass::GetLightSphereData(Light& light, Camera& camera)
	{
		glm::vec3 center = glm::vec3(0.0f);
		float radius = 0;
		if (light.Type == LightType::Point)
		{
			glm::vec4 posWS = glm::vec4(light.GetTransform().GetPosition(), 1.0f);
			glm::vec4 posVS = camera.GetViewMatrix() * posWS;
			center = posVS;
			radius = light.LightRange;
		}
		else if (light.Type == LightType::Spot)
		{
			glm::vec4 posWS = glm::vec4(light.GetTransform().GetPosition(), 1.0f);
			glm::vec4 posVS = camera.GetViewMatrix() * posWS;
			glm::vec4 dirWS = glm::vec4(light.GetTransform().GetForward(), 0.0f);
			glm::vec4 dirVS = camera.GetViewMatrix() * dirWS;
			float spotlightAngle = glm::radians(light.SpotAngle * 0.5f);
			//最小包围球
			//https://ubm-twvideo01.s3.amazonaws.com/o1/vault/gdc2015/presentations/Thomas_Gareth_Advancements_in_Tile-Based.pdf 示意图
			//https://wickedengine.net/2018/01/10/optimizing-tile-based-light-culling/
			float spotLightConeHalfAngleCos = std::cos(spotlightAngle);
			float sphereRadius = light.LightRange * 0.5f / (spotLightConeHalfAngleCos * spotLightConeHalfAngleCos); //锐角三角形最小覆盖圆为其外接圆
			glm::vec4 sphereCenterVS = posVS + dirVS * sphereRadius;
			center = sphereCenterVS;
			radius = sphereRadius;
		}
		else if(light.Type == LightType::Directional)
		{
			center = glm::vec3(0.0f);
			radius = camera.m_FarPlane * 2.0f;
		}

		return glm::vec4(center, radius);
	}
}
