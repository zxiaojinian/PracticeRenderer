#include "pch.h"
#include "Texture2D.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLTexture.h"

namespace PR
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& name, const Texture2DSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
		{
			PR_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(name, specification);
		}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
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
			return std::make_shared<OpenGLTexture2D>(path);
		}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
