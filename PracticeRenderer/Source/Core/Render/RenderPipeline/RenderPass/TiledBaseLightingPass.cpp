#include "pch.h"
#include "TiledBaseLightingPass.h"

#include <glm/gtc/matrix_inverse.hpp>

namespace PR
{
	TiledBaseLightingPass::TiledBaseLightingPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		m_DepthBoundsCS = ComputeShader::Create("Assets/Shader/LightCulling/TiledCulling/DepthBounds.compute");

		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureFormat Format = TextureFormat::R8G8B8A8_SRGB;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
		TextureFilterMode FilterMode = TextureFilterMode::Trilinear;
		bool GenerateMips = true;

		//Texture2DSpecification specification{};
		//specification.Width = 
		//m_DepthBounds = Texture2D::Create("depthBounds", )
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
		}

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
		}

		if (cameraData.camera && m_PreProjectionMatrix != cameraData.camera->GetProjectionMatrix())
		{
			m_PreProjectionMatrix = cameraData.camera->GetProjectionMatrix();
			m_InvProjectionMatrix = glm::inverse(m_PreProjectionMatrix);
		}

		m_DepthBoundsCS->SetFloat4("ScreenParams_CS", m_ScreenParams);
		m_DepthBoundsCS->SetMat4("InverseProjection", m_InvProjectionMatrix);
		m_DepthBoundsCS->SetTexture("depthBounds", m_DepthBounds.get());
		graphicsContext.DispatchCompute(*m_DepthBoundsCS, m_TileCountX, m_TileCountY, 1);
	}
}
