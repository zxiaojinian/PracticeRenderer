#include "Core/pch.h"
#include "OpenGLContext.h"

#include "Core/Common.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PR
{
	OpenGLContext::OpenGLContext()
	{
		Init();
	}

	void OpenGLContext::Init()
	{
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PR_ASSERT(status, "Failed to initialize Glad!");

		//TODO,(const char*)
		PR_INFO("OpenGL Info:");
		PR_INFO("  Vendor: {0}", (const char*)(glGetString(GL_VENDOR)));
		PR_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		PR_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}
}