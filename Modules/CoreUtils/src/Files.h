#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdio.h>
#include <assert.h>

namespace CoreUtils
{
   bool CheckIfFolderExist(std::string folderPath);
   bool CreateFolder(std::string folderPath);

   bool CheckIfFileExist(std::string filePath);

   bool CheckIfPathEndsWithSeparator(const std::string& path_str);

   bool ReadFile(const char* pFileName, std::string &outFile);
   char* ReadBinaryFile(const char* pFilename, int& size);
   void WriteBinaryFile(const char *pFilename, const void* pData, int size);
}
