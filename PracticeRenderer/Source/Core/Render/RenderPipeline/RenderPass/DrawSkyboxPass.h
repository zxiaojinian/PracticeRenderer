#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"
#include "Core/Render/Asset/Material.h"
#include "Core/Render/Asset/Mesh.h"
#include "Core/Render/Cubemap.h"

namespace PR
{
	class DrawSkyboxPass : public RenderPass
	{
	public:
		DrawSkyboxPass(RenderPassEvent renderPassEvent);

		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

	private:
		std::shared_ptr<Cubemap> skyCubeMap = nullptr;
		std::shared_ptr<Material> m_SkyBoxMaterial = nullptr;
		std::shared_ptr<Mesh> m_SkyMesh = nullptr;
	};
}


