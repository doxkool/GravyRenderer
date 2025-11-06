#include "Graphics\Renderer\Vulkan\VkShader.h"

#include "Files.h"

namespace VkCore
{
    VkDevice m_Device;

    struct Shader
    {
    	std::vector<uint32_t> SPIRV;
    	VkShaderModule ShaderModule = NULL;

    	void Init(glslang_program_t* program)
    	{
    		size_t program_size = glslang_program_SPIRV_get_size(program);
    		SPIRV.resize(program_size);
    		glslang_program_SPIRV_get(program, SPIRV.data());
    	}
    };

    Shader m_ShaderModule;

    static glslang_stage_t ShaderStageFromFile(const char* pFilename)
    {
    	std::string s(pFilename);
    
    	if (s.ends_with(".vert")) {
    		return GLSLANG_STAGE_VERTEX;
    	}
    
    	if (s.ends_with(".frag")) {
    		return GLSLANG_STAGE_FRAGMENT;
    	}
    
    	if (s.ends_with(".geom")) {
    		return GLSLANG_STAGE_GEOMETRY;
    	}
    
    	if (s.ends_with(".comp")) {
    		return GLSLANG_STAGE_COMPUTE;
    	}
    
    	if (s.ends_with(".tesc")) {
    		return GLSLANG_STAGE_TESSCONTROL;
    	}
    
    	if (s.ends_with(".tese")) {
    		return GLSLANG_STAGE_TESSEVALUATION;
    	}
    
    	printf("Unknown shader stage in '%s'\n", pFilename);
    	exit(1);
    
    	return GLSLANG_STAGE_VERTEX;
    }

    static void PrintShaderSource(const char* text)
    {
    	int line = 1;

    	printf("\n(%3i) ", line);

    	while (text && *text++)
    	{
    		if (*text == '\n') {
    			printf("\n(%3i) ", ++line);
    		}
    		else if (*text == '\r') {
    			// nothing to do
    		}
    		else {
    			printf("%c", *text);
    		}
    	}

    	printf("\n");
    }

    static bool CompileShader(VkDevice Device, glslang_stage_t Stage, const char* pShaderCode, Shader& ShaderModule)
    {
    	glslang_input_t input = {
    		.language = GLSLANG_SOURCE_GLSL,
    		.stage = Stage,
    		.client = GLSLANG_CLIENT_VULKAN,
    		.client_version = GLSLANG_TARGET_VULKAN_1_3,
    		.target_language = GLSLANG_TARGET_SPV,
    		.target_language_version = GLSLANG_TARGET_SPV_1_6,
    		.code = pShaderCode,
    		.default_version = 100,
    		.default_profile = GLSLANG_NO_PROFILE,
    		.force_default_version_and_profile = false,
    		.forward_compatible = false,
    		.messages = GLSLANG_MSG_DEFAULT_BIT,
    		.resource = glslang_default_resource()
    	};

    	glslang_shader_t* shader = glslang_shader_create(&input);

    	if (!glslang_shader_preprocess(shader, &input))	{
    		fprintf(stderr, "GLSL preprocessing failed\n");
    		fprintf(stderr, "\n%s", glslang_shader_get_info_log(shader));
    		fprintf(stderr, "\n%s", glslang_shader_get_info_debug_log(shader));
    		PrintShaderSource(input.code);
    		return 0;
    	}

    	if (!glslang_shader_parse(shader, &input)) {
    		fprintf(stderr, "GLSL parsing failed\n");
    		fprintf(stderr, "\n%s", glslang_shader_get_info_log(shader));
    		fprintf(stderr, "\n%s", glslang_shader_get_info_debug_log(shader));
    		PrintShaderSource(glslang_shader_get_preprocessed_code(shader));
    		return 0;
    	}

    	glslang_program_t* program = glslang_program_create();
    	glslang_program_add_shader(program, shader);

    	if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT)) {
    		fprintf(stderr, "GLSL linking failed\n");
    		fprintf(stderr, "\n%s", glslang_program_get_info_log(program));
    		fprintf(stderr, "\n%s", glslang_program_get_info_debug_log(program));
    		return 0;
    	}

    	glslang_program_SPIRV_generate(program, Stage);

    	ShaderModule.Init(program);

    	const char* spirv_messages = glslang_program_SPIRV_get_messages(program);

    	if (spirv_messages) {
    		fprintf(stderr, "SPIR-V message: '%s'", spirv_messages);
    	}

    	VkShaderModuleCreateInfo shaderCreateInfo = {
    		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    		.codeSize = ShaderModule.SPIRV.size() * sizeof(uint32_t),
    		.pCode = (const uint32_t*)ShaderModule.SPIRV.data()
    	};

    	VkResult res = vkCreateShaderModule(Device, &shaderCreateInfo, NULL, &ShaderModule.ShaderModule);
    	CHECK_VK_RESULT(res, "vkCreateShaderModule\n");

    	glslang_program_delete(program);
    	glslang_shader_delete(shader);

    	bool ret = ShaderModule.SPIRV.size() > 0;

    	return ret;
    }

    VkShaderModule CreateFromBinary(VkDevice& device, const char* pShaderPath)
    {
        int codeSize = 0;
		char* pShaderCode = ReadBinaryFile(pShaderPath, codeSize);
		assert(pShaderCode);

		VkShaderModuleCreateInfo shaderCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = (size_t)codeSize,
			.pCode = (const uint32_t*)pShaderCode
		};

		VkShaderModule shaderModule;
		VkResult res = vkCreateShaderModule(device, &shaderCreateInfo, NULL, &shaderModule);
		CHECK_VK_RESULT(res, "vkCreateShaderModule\n");
		VKCORE_DEBUG("Created shader from binary %s\n", pShaderPath);

		free(pShaderCode);

		return shaderModule;
    }

    VkShaderModule CreateFromFile(VkDevice& device, const char* pShaderPath)
    {
        std::string source;

        if (!CoreUtils::ReadFile(pShaderPath, source))
        {
            assert(0);
        }
        
        glslang_stage_t shaderStage = ShaderStageFromFile(pShaderPath);

        VkShaderModule shaderModule = nullptr;

        glslang_initialize_process();

        bool success = CompileShader(device, shaderStage, source.c_str(), m_ShaderModule);

        if (success)
        {
			VKCORE_DEBUG("Shader compiled successfully: {}", pShaderPath);
			shaderModule = m_ShaderModule.ShaderModule;
			std::string BinaryFileName = std::string(pShaderPath) + ".spv";
			std::ofstream BinaryFile(BinaryFileName, std::ios::binary);
			CoreUtils::WriteBinaryFile(BinaryFileName.c_str(), m_ShaderModule.SPIRV.data(), m_ShaderModule.SPIRV.size() * sizeof(uint32_t));
		}else{
			VKCORE_ERROR("Shader compilation failed: {}", pShaderPath);
		}

        glslang_finalize_process();

        return shaderModule;
    }

    void Destroy()
    {
        
    }

    VkShaderModule GetShaderModule()
    {
        return m_ShaderModule.ShaderModule;
    }
}