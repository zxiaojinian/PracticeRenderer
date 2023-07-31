#pragma once

#include <memory>

namespace PR
{
	enum class BackendsAPI
	{
		None = 0,
		OpenGL = 1
	};

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		static BackendsAPI s_BackendAPI;

		static std::unique_ptr<GraphicsContext> Create();
	};
}
