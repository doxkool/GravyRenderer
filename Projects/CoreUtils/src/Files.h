#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdio.h>

namespace CoreUtils
{
   bool CheckIfFolderExist(std::string folderPath);
   bool CreateFolder(std::string folderPath);

   bool CheckIfFileExist(std::string filePath);

   bool CheckIfPathEndsWithSeparator(const std::string& path_str);

   int SaveBinaryToDisk(std::vector<char> binaryData, int length, std::string outputPath);
   std::vector<char> ReadBinaryFromDisk(std::string filePath);
}
