#include "pch.h"
#include "VertexBuffer.h"

#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLVertexBuffer.h"

namespace PR
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
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
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
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
				return std::make_shared<OpenGLVertexBuffer>(size);
			}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
