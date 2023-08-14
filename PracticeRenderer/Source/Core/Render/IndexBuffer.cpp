#include "pch.h"
#include "IndexBuffer.h"

#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLIndexBuffer.h"

namespace PR
{
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
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
				return std::make_shared<OpenGLIndexBuffer>(indices, count);
			}		
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
