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

		static std::unique_ptr<GraphicsContext> Create(BackendsAPI api);
	};
}
