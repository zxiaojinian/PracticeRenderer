#pragma once

#include "Core/Log/Log.h"

#ifdef PR_DEBUG
	#define PR_ASSERT(x, ...) { if(!(x)) { PR_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PR_ASSERT(x, str)
#endif

//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIND_EVENT_PTR_FN(ptr,fn) [this](auto&&... args) -> decltype(auto) { return this->ptr->fn(std::forward<decltype(args)>(args)...); }
#define BIND_EVENT_OBJECT_FN(obj,fn) [this](auto&&... args) -> decltype(auto) { return this->obj.fn(std::forward<decltype(args)>(args)...); }