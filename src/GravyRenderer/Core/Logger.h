#pragma once

#include <spdlog/spdlog.h>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

class Logger
{
private:
    
public:
    static int Init(std::string app_name = "Gravy");
    static void Set_LogLevel(int log_level);
    static std::shared_ptr<spdlog::logger>& GetLogger();
};

#define LOG_TRACE(...)				Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)				Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)				Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)				Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)				Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)			Logger::GetLogger()->critical(__VA_ARGS__)