#include "pch.h"
#include "VertexArray.h"

#include "Core/Common.h"
#include "Core/Render/OpenGL/OpenGLVertexArray.h"
#include "Core/Render/RendererAPI.h"

namespace PR
{
	std::shared_ptr<VertexArray> VertexArray::Create()
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
				return std::make_shared<OpenGLVertexArray>();
			}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
