#pragma once

#include "Core/Log/Log.h"

#ifdef PR_DEBUG
	#if defined(PR_PLATFORM_WINDOWS)
		#define PR_DEBUGBREAK() __debugbreak()
	#else
		#define PR_DEBUGBREAK()
	#endif

	#define PR_ENABLE_ASSERTS
#else
	#define PR_DEBUGBREAK()
#endif

#ifdef PR_ENABLE_ASSERTS
	#define PR_ASSERT(x, ...) { if(!(x)) { PR_ERROR("Assertion Failed: {0}", __VA_ARGS__); PR_DEBUGBREAK(); } }
#else
	#define PR_ASSERT(x, str)
#endif

//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
