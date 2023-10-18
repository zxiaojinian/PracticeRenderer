#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Render/Texture2D.h"
#include "Core/Render/Asset/ComputeShader.h"

#include <glm/glm.hpp>

namespace PR
{
	class TiledBaseLightingPass : public RenderPass
	{
	public:
		TiledBaseLightingPass(RenderPassEvent renderPassEvent);

		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

	private:
		const uint32_t TILE_SIZE = 16;

		uint32_t m_PixelWidth = 1;
		uint32_t m_pixelHeight = 1;
		uint32_t m_TileCountX = 0;
		uint32_t m_TileCountY = 0;
		uint32_t m_TileCount = 0;
		glm::vec4 m_ScreenParams = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 m_PreProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_InvProjectionMatrix = glm::mat4(1.0f);

		std::shared_ptr<Texture2D> m_DepthBounds = nullptr;
		std::shared_ptr<ComputeShader> m_DepthBoundsCS = nullptr;
	};
}

