#include "Core/pch.h"
#include "GraphicsContext.h"

#include "Core/Common.h"
#include "Core/Platform/OpenGL/OpenGLContext.h"

namespace PR
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(BackendsAPI api)
	{
		switch (api)
		{
			case BackendsAPI::None:    
			{
				PR_ASSERT(false, "BackendsAPI::None is currently not supported!");
				return nullptr;
			}
			case BackendsAPI::OpenGL:
			{
				return std::make_unique<OpenGLContext>();
			}
		}

		PR_ASSERT(false, "Unknown BackendsAPI!");
		return nullptr;
	}

}