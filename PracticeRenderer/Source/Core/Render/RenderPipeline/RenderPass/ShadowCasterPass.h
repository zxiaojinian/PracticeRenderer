#pragma once

#include "Core/Render/RenderPipeline/RenderPass.h"

#include <glm/glm.hpp>

namespace PR
{
	class ShadowCasterPass : RenderPass
	{
	public:
		ShadowCasterPass(RenderPassEvent renderPassEvent);

		virtual void Configure() override;
		virtual void Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData) override;

		bool Setup(const RenderingData& renderingData);

	private:
		std::shared_ptr<RenderTexture> m_Shadowmap;
		std::shared_ptr<Material> m_ShadowCasterMat;

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};
}

