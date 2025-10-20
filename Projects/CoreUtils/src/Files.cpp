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

    int SaveBinaryToDisk(std::vector<char> binaryData, int length, std::string outputPath)
    {
        std::ofstream outFile(outputPath, std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(reinterpret_cast<char*>(binaryData.data()), length);
            outFile.close();
            return 0;
        } else {
            return 1;
        }
    }

    std::vector<char> ReadBinaryFromDisk(std::string filePath)
    {
        std::vector<char> buffer;

        std::ifstream inputFile(filePath, std::ios::binary | std::ios::ate); 

        if (inputFile.is_open()) {
            // Get the size of the file
            std::streampos fileSize = inputFile.tellg();
            inputFile.seekg(0, std::ios::beg); // Seek back to the beginning of the file

            // Create a buffer (e.g., a vector of chars) to store the data
            buffer.resize(fileSize);

            // Read the entire file into the buffer
            inputFile.read(buffer.data(), fileSize);

            // Close the file
            inputFile.close();

            return buffer;
        }

        return buffer;
    }
}
