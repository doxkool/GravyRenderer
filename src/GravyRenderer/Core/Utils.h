#pragma once

#include "Core/Base.h"
#include "Core/Logger.h"

namespace Utils
{
    bool CheckIfFolderExist(std::string folderPath);
    bool CreateFolder(std::string folderPath);

    bool CheckIfFileExist(std::string filePath);

    bool CheckIfPathEndsWithSeparator(const std::string& path_str);
}
