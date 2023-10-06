#include "pch.h"
#include "Cubemap.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLCubemap.h"

namespace PR
{
	std::shared_ptr<Cubemap> Cubemap::Create(const std::string& name, const CubemapSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
			{
				PR_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLCubemap>(name, specification);
			}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
