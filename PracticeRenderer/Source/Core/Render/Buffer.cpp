#include "pch.h"
#include "Buffer.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLBuffer.h"

namespace PR
{
	std::shared_ptr<Buffer> Buffer::Create(uint32_t count, uint32_t stride, BufferType bufferType, BufferUsage bufferUsage)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
		{
			PR_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLBuffer>(count, stride, bufferType, bufferUsage);
		}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
