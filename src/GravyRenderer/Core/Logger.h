#pragma once

#include <spdlog/spdlog.h>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Gravy
{
    class Logger
    {
    private:
        
    public:
        static int Init(std::string app_name = "Gravy");
	    static void Set_LogLevel(int log_level);
	    static std::shared_ptr<spdlog::logger>& GetLogger();
    };    
}

#define LOG_TRACE(...)				Gravy::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)				Gravy::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)				Gravy::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)				Gravy::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)				Gravy::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)			Gravy::Logger::GetLogger()->critical(__VA_ARGS__)