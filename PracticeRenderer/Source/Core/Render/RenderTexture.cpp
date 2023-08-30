#include "pch.h"
#include "RenderTexture.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLRenderTexture.h"

namespace PR
{
    std::shared_ptr<RenderTexture> RenderTexture::Create(const std::string& name, const RenderTextureSpecification& specification)
    {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
			{
				PR_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLRenderTexture>(name, specification);
			}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}