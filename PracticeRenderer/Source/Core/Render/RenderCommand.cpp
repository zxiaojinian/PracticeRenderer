#include "pch.h"
#include "RenderCommand.h"

namespace PR
{
	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
