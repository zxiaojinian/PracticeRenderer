#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace PR
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#define PR_TRACE(...)		::PR::Log::GetLogger()->trace(__VA_ARGS__)
#define PR_DEBUG(...)		::PR::Log::GetLogger()->debug(__VA_ARGS__)
#define PR_INFO(...)		::PR::Log::GetLogger()->info(__VA_ARGS__)
#define PR_WARN(...)		::PR::Log::GetLogger()->warn(__VA_ARGS__)
#define PR_ERROR(...)		::PR::Log::GetLogger()->error(__VA_ARGS__)
#define PR_CRITICAL(...)	::PR::Log::GetLogger()->critical(__VA_ARGS__)

