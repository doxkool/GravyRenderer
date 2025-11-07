#pragma once

#include "Logger.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    typedef unsigned int uint;

    void ParseVkResult(VkResult res, const char* pFile, int line, const char* msg);

    #define CHECK_VK_RESULT(res, msg) ParseVkResult(res, __FILE__, __LINE__, msg) // { LOG_ERROR("Error in {}:{} - {}, code {}", __FILE__, __LINE__, msg, (int)res); }

    const char* GetDebugSeverityStr(VkDebugUtilsMessageSeverityFlagBitsEXT Severity);
    const char* GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type);

    bool ReadFile(const char *pFileName, std::string& outFile);
    char* ReadBinaryFile(const char* pFilename, int& size);
    void WriteBinaryFile(const char* pFilename, const void* pData, int size);
}