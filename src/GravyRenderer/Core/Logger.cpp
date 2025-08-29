#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Gravy
{

    std::shared_ptr<spdlog::logger> s_Logger;

    std::string appName;

    int Logger::Init(std::string app_name)
    {
        appName = app_name;
        std::vector<spdlog::sink_ptr> Sinks;
        Sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        Sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Renderer_log.txt", true));
        Sinks[0]->set_pattern("[%T] [%^%l%$]	[%n]   	%v");
        Sinks[1]->set_pattern("[%T] [%l]	[%n]   	%v");

        s_Logger = std::make_shared<spdlog::logger>(appName.c_str(), begin(Sinks), end(Sinks));
        spdlog::register_logger(s_Logger);
        s_Logger->set_level(spdlog::level::trace);
        s_Logger->flush_on(spdlog::level::trace);

        LOG_INFO("\n====== New logging session ======");
        if (GetLogger)
        {
            LOG_DEBUG("Logger is ready!");
        }
        return 0;
    }
    // Set the logging level for the Renderer | 0 = Trace, 1 = Debug and 2 = Info.
    void Logger::Set_LogLevel(int log_level)
    {
        if (log_level == 0)
        {
            s_Logger->set_level(spdlog::level::trace);
            s_Logger->flush_on(spdlog::level::trace);
            LOG_INFO("Log level has been changed to TRACE");
        }
        if (log_level == 1)
        {
            s_Logger->set_level(spdlog::level::debug);
            s_Logger->flush_on(spdlog::level::debug);
            LOG_INFO("Log level has been changed to DEBUG");
        }
        if (log_level == 2)
        {
            s_Logger->set_level(spdlog::level::info);
            s_Logger->flush_on(spdlog::level::info);
            LOG_INFO("Log level has been changed to INFO");
        }
    }

    std::shared_ptr<spdlog::logger> &Logger::GetLogger()
    {
        return s_Logger;
    }

}