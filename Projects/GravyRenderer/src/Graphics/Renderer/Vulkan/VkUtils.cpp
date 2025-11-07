#include "Graphics\Renderer\Vulkan\VkUtils.h"

namespace VkCore
{
	void ParseVkResult(VkResult res, const char* pFile, int line, const char* msg)
	{
		switch (res)
		{
		case VK_SUCCESS:
			return;

		case VK_NOT_READY:
			LOG_WARN("VK_NOT_READY in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		case VK_TIMEOUT:
			LOG_WARN("VK_TIMEOUT in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		case VK_EVENT_SET:
			LOG_WARN("VK_EVENT_SET in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		case VK_EVENT_RESET:
			LOG_WARN("VK_EVENT_RESET in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		case VK_INCOMPLETE:
			LOG_WARN("VK_INCOMPLETE in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		case VK_ERROR_OUT_OF_HOST_MEMORY:
			LOG_WARN("VK_ERROR_OUT_OF_HOST_MEMORY in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			LOG_WARN("VK_ERROR_OUT_OF_DEVICE_MEMORY in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		case VK_ERROR_INITIALIZATION_FAILED:
			LOG_WARN("VK_ERROR_INITIALIZATION_FAILED in {}:{} - {} - Code {}", pFile, line, msg, (int)res);

		default:
			LOG_ERROR("Error in {}:{} - {} - Code {}", pFile, line, msg, (int)res);
		}
	}

	const char *GetDebugSeverityStr(VkDebugUtilsMessageSeverityFlagBitsEXT Severity)
	{
		switch (Severity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			return "Verbose";

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			return "Info";

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			return "Warning";

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			return "Error";

		default:
			exit(1);
		}

		return "NO SUCH SEVERITY!";
	}

	const char *GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type)
	{
		switch (Type)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			return "General";

		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			return "Validation";

		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			return "Performance";

#ifdef _WIN64 // doesn't work on my Linux for some reason
		case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
			return "Device address binding";
#endif

		default:
			exit(1);
		}

		return "NO SUCH TYPE!";
	}

	bool ReadFile(const char* pFileName, std::string &outFile)
	{
		std::ifstream f(pFileName);

		bool ret = false;

		if (f.is_open())
		{
			std::string line;
			while (getline(f, line))
			{
				outFile.append(line);
				outFile.append("\n");
			}
			f.close();

			ret = true;
		}
		else
		{
			printf("CANNOT_READ_FILE: {}", pFileName);
		}

		return ret;
	}

    char* ReadBinaryFile(const char* pFilename, int& size)
	{
		FILE *f = fopen(pFilename, "rb");

		if (!f)
		{
			printf("Error opening '%s': %s\n", pFilename, strerror(errno));
			exit(0);
		}

		struct stat stat_buf;
		int error = stat(pFilename, &stat_buf);

		if (error)
		{
			printf("Error getting file stats: %s\n", strerror(errno));
			return NULL;
		}

		size = stat_buf.st_size;

		char *p = (char *)malloc(size);
		assert(p);

		size_t bytes_read = fread(p, 1, size, f);

		if (bytes_read != size)
		{
			printf("Read file error file: %s\n", strerror(errno));
			exit(0);
		}

		fclose(f);

		return p;
	}

	void WriteBinaryFile(const char *pFilename, const void* pData, int size)
	{
		FILE *f = fopen(pFilename, "wb");

		if (!f)
		{
			printf("Error opening '%s': %s\n", pFilename, strerror(errno));
			exit(0);
		}

		int bytes_written = fwrite(pData, 1, size, f);

		if (bytes_written != size)
		{
			printf("Error write file: %s\n", strerror(errno));
			exit(0);
		}

		fclose(f);
	}
}