#include "Core/Utils.h"

namespace Utils
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
        if (!CheckIfFolderExist(folderPath)) {
            if (std::filesystem::create_directory(folderPath)) {
                LOG_TRACE("Folder created successfully: {}", folderPath);
                return true;
            } else {
                LOG_ERROR("Error creating folder: {}", folderPath);
                return false;
            }
        }else{
            LOG_WARN("The folder already exist: {}", folderPath);
            return true;
        }

        return false;
    }

    bool CheckIfFileExist(std::string filePath)
    {
        return false;
    }

    bool CheckIfPathEndsWithSeparator(const std::string& path_str) {
        std::filesystem::path p(path_str);
        // Use the generic_string() to normalize backslashes to forward slashes for consistent checking
        // and then check the last character.
        if (!p.empty()) {
            char last_char = p.generic_string().back();
            return (last_char == '/' || last_char == '\\'); 
        }
        return false;
    }
}
