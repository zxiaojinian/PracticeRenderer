#include "pch.h"
#include "Framebuffer.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLFramebuffer.h"

namespace PR
{
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
			{
				PR_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLFramebuffer>(spec);
			}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}