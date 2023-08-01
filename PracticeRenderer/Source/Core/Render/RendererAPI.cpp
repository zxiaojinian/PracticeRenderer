#include "pch.h"
#include "RendererAPI.h"

#include "Core/Common.h"
#include "Core/Render/OpenGL/OpenGLRendererAPI.h"

namespace PR
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    
			{
				PR_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:  
			{
				return std::make_unique<OpenGLRendererAPI>();
			}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
