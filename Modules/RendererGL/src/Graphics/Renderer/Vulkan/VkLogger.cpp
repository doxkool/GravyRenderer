#include "Graphics\Renderer\Vulkan\VkLogger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace VkCore
{
	std::shared_ptr<spdlog::logger> s_VkCoreLogger;

	std::string appName;

	void Logger::Init(std::string app_name)
	{
		appName = app_name;
		std::vector<spdlog::sink_ptr> Sinks;
		Sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		Sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Graphics\Renderer\Vulkan\Vklog.txt", true));
		Sinks[0]->set_pattern("[%T] [%^%l%$]	[%n]   	%v");
		Sinks[1]->set_pattern("[%T] [%l]	[%n]   	%v");

		s_VkCoreLogger = std::make_shared<spdlog::logger>(appName.c_str(), begin(Sinks), end(Sinks));
		spdlog::register_logger(s_VkCoreLogger);
		s_VkCoreLogger->set_level(spdlog::level::trace);
		s_VkCoreLogger->flush_on(spdlog::level::trace);

		VKCORE_INFO("\n====== New logging session ======");
		if (GetVkCoreLogger)
		{
			VKCORE_DEBUG("VkCore logger is ready!");
		}
	}
	// Set the logging level for the Renderer | 0 = Trace, 1 = Debug and 2 = Info.
	void Logger::Set_VkCore_LogLevel(int log_level)
	{
		if (log_level == 0)
		{
			s_VkCoreLogger->set_level(spdlog::level::trace);
			s_VkCoreLogger->flush_on(spdlog::level::trace);
			VKCORE_INFO("Log level has been changed to TRACE");
		}
		if (log_level == 1)
		{
			s_VkCoreLogger->set_level(spdlog::level::debug);
			s_VkCoreLogger->flush_on(spdlog::level::debug);
			VKCORE_INFO("Log level has been changed to DEBUG");
		}
		if (log_level == 2)
		{
			s_VkCoreLogger->set_level(spdlog::level::info);
			s_VkCoreLogger->flush_on(spdlog::level::info);
			VKCORE_INFO("Log level has been changed to INFO");
		}
	}

	std::shared_ptr<spdlog::logger> &Logger::GetVkCoreLogger()
	{
		return s_VkCoreLogger;
	}
}