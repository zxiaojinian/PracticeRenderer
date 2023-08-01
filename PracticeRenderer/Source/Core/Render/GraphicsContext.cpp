#include "pch.h"
#include "GraphicsContext.h"

#include "Core/Common.h"
#include "Core/Render/OpenGL/OpenGLContext.h"
#include "Core/Render/RendererAPI.h"

namespace PR
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    
			{
				PR_ASSERT(false, "BackendsAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_unique<OpenGLContext>();
			}
		}

		PR_ASSERT(false, "Unknown BackendsAPI!");
		return nullptr;
	}

}