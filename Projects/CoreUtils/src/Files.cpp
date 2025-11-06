#include "Files.h"

namespace CoreUtils
{
    bool CheckIfFolderExist(std::string folderPath)
    {
        if (std::filesystem::exists(folderPath))
        {
            return true;
        }

        return false;
    }

    bool CreateFolder(std::string folderPath)
    {
        if (std::filesystem::create_directory(folderPath)) {
            return true;
        } else {
            return false;
        }

        return false;
    }

    bool CheckIfFileExist(std::string filePath)
    {
        if (std::filesystem::exists(filePath)) {
            return true;
        } else {
            return false;
        }

        return false;
    }

    bool CheckIfPathEndsWithSeparator(const std::string& path_str)
    {
        std::filesystem::path p(path_str);

        if (!p.empty()) {
            char last_char = p.generic_string().back();
            return (last_char == '/' || last_char == '\\'); 
        }
        return false;
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
