#include "Core/pch.h"
#include "Window.h"

#include "Core/Common.h"

#ifdef PR_PLATFORM_WINDOWS
	#include  "Platform/Windows/WindowsWindow.h"
#endif

namespace PR
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
#ifdef PR_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
#else
		PR_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}
