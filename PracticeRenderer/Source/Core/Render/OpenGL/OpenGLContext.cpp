#include "pch.h"
#include "OpenGLContext.h"

#include "Core/Common.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PR
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         PR_LOG_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       PR_LOG_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          PR_LOG_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: PR_LOG_TRACE(message); return;
		}

		PR_ASSERT(false, "Unknown severity level!");
	}

	OpenGLContext::OpenGLContext()
	{
		Init();
	}

	void OpenGLContext::Init()
	{
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PR_ASSERT(status, "Failed to initialize Glad!");

		//TODO,(const char*)
		PR_LOG_INFO("OpenGL Info:");
		PR_LOG_INFO("Vendor: {0}", (const char*)(glGetString(GL_VENDOR)));
		PR_LOG_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		PR_LOG_INFO("Version: {0}", (const char*)glGetString(GL_VERSION));

#if PR_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
	}
}
