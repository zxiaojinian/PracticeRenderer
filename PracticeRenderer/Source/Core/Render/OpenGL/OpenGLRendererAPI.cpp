#include "pch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace PR
{
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const Color& color)
	{
		glClearColor(color.R, color.G, color.B, color.A);
	}

	void OpenGLRendererAPI::Clear(bool clearDepth, bool clearColor)
	{
		if (clearColor && clearDepth)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (clearColor)
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (clearDepth)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	void OpenGLRendererAPI::DrawIndexed(uint32_t indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}