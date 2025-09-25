#include "Shader.h"

namespace Renderer
{
    void Shader::LoadBinaryFromDisk(std::string ShaderPath)
    {
        bool folderExist = Utils::CheckIfFolderExist(ShaderPath);

        
    }
	
    void Shader::SaveBinaryToDisk(std::string path)
    {
        bool folderExist = Utils::CheckIfFolderExist(path);

        if (!Utils::CheckIfPathEndsWithSeparator(path)) {
            path = path + "/";
        }

        if (!folderExist) {
            if(Utils::CreateFolder(path)) {
                SaveToDisk(path);
            }
        }else{
            SaveToDisk(path);
        }
    }
}