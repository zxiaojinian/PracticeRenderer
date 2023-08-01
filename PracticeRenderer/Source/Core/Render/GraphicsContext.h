#pragma once

#include "Core/Render/VertexArray.h"

#include <glm/glm.hpp>

#include <memory>

namespace PR
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

	public:
		static std::unique_ptr<GraphicsContext> Create();
	};
}
