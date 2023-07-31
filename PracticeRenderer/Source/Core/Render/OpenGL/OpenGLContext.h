#pragma once

#include "Core/Render/GraphicsContext.h"

namespace PR
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext();

	private:
		void Init();
	};
}

