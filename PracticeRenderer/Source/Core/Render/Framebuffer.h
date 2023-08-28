#pragma once

#include "Core/Render/RenderTexture.h"

#include <initializer_list>
#include <vector>
#include <memory>

namespace PR
{
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void AttachColorTexture(RenderTexture& colorRT) = 0;
		virtual void AttachColorTexture(std::vector<RenderTexture&>& colorRTs) = 0;
		virtual void AttachDepthTexture(RenderTexture& depthRT) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static std::unique_ptr<Framebuffer> Create();
	};
}

