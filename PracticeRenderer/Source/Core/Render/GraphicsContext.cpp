#include "pch.h"
#include "GraphicsContext.h"

#include "Core/Common.h"
#include "Core/Render/OpenGL/OpenGLContext.h"

namespace PR
{
	BackendsAPI GraphicsContext::s_BackendAPI = BackendsAPI::OpenGL;

	std::unique_ptr<GraphicsContext> GraphicsContext::Create()
	{
		switch (s_BackendAPI)
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