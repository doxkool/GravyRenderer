#pragma once

#include <spdlog/spdlog.h>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace VkCore
{
	class Logger
	{
	public:
		static void Init(std::string app_name = "VkCore");
		static void Set_VkCore_LogLevel(int log_level);
		static std::shared_ptr<spdlog::logger> &GetVkCoreLogger();

	private:
	};
}

// Core log macros
#define VKCORE_TRACE(...) 		VkCore::Logger::GetVkCoreLogger()->trace(__VA_ARGS__)
#define VKCORE_DEBUG(...) 		VkCore::Logger::GetVkCoreLogger()->debug(__VA_ARGS__)
#define VKCORE_INFO(...) 		VkCore::Logger::GetVkCoreLogger()->info(__VA_ARGS__)
#define VKCORE_WARN(...) 		VkCore::Logger::GetVkCoreLogger()->warn(__VA_ARGS__)
#define VKCORE_ERROR(...) 		VkCore::Logger::GetVkCoreLogger()->error(__VA_ARGS__)
#define VKCORE_CRITICAL(...) 	VkCore::Logger::GetVkCoreLogger()->critical(__VA_ARGS__)