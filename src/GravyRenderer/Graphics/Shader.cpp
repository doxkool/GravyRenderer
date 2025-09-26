#include "Shader.h"

#include "CoreUtils.h"

namespace Renderer
{
    void Shader::LoadBinaryFromDisk(std::string ShaderPath)
    {
        bool folderExist = CoreUtils::CheckIfFolderExist(ShaderPath);

        auto data = CoreUtils::ReadBinaryFromDisk(ShaderPath);
        LoadShader(data, 0);
    }
	
    void Shader::SaveBinaryToDisk(std::string path)
    {
        if (!CoreUtils::CheckIfPathEndsWithSeparator(path)) {
            path = path + "/";
        }

        auto binary = GetShaderBinary();

        if (!CoreUtils::CheckIfFolderExist(path)) {
            CoreUtils::CreateFolder(path);
            //CoreUtils::SaveBinaryToDisk(reinterpret_cast<char*>(binary.data()),);
            SaveToDisk(path);
        }else{
            SaveToDisk(path);
        }
    }
}