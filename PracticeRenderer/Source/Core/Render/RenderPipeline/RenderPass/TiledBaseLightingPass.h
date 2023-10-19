#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Render/Texture2D.h"
#include "Core/Render/Asset/ComputeShader.h"
#include "Core/Render/Buffer.h"
#include "Core/Scene/Components/Light.h"
#include "Core/Scene/Components/Camera.h"

#include <glm/glm.hpp>

namespace PR
{
	class TiledBaseLightingPass : public RenderPass
	{
	public:
		TiledBaseLightingPass(RenderPassEvent renderPassEvent);
		void Setup(bool debug) { m_Debug = debug; }
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

	private:
		glm::vec4 GetLightSphereData(Light& light, Camera& camera);

	private:
		const uint32_t TILE_SIZE = 16;
		const uint32_t MAX_NUM_LIGHTS_PER_TILE = 32;

		bool m_Debug = true;

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
		std::shared_ptr<ComputeShader> m_LightCullingCS = nullptr;
		std::shared_ptr<ComputeShader> m_Debug_LightCullingCS = nullptr;
		std::shared_ptr<Buffer> m_LightCullingDataBuffer = nullptr;
		std::shared_ptr<Buffer> m_LightIndexListDoubleBuffer = nullptr;
		std::shared_ptr<Texture2D> m_DebugTexture = nullptr;
	};
}

