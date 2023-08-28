#include "pch.h"
#include "Framebuffer.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLFramebuffer.h"

namespace PR
{
	std::unique_ptr<Framebuffer> Framebuffer::Create()
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
				return std::make_unique<OpenGLFramebuffer>();
			}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}