#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

#include "Utils/Structs.h"

class GP2Shader final 
{
public:
	GP2Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) : 
		m_VertexShaderFile{ vertexShaderFile },
		m_FragmentShaderFile{ fragmentShaderFile },
		m_VecShadersStageInfos{  },
		m_InputBinding{Vertex::GetBindingDescription()}, 
		m_AttributeDescriptions{ Vertex::GetAttributeDescriptions() }
	{}

	~GP2Shader() = default;

	GP2Shader(const GP2Shader& other) = delete;
	GP2Shader(GP2Shader&& other) noexcept = delete;
	GP2Shader& operator=(const GP2Shader& other) = delete;
	GP2Shader& operator=(GP2Shader&& other) noexcept = delete;
	
	void Init(const VkDevice& device);

	const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStageInfos() { return m_VecShadersStageInfos; }
	void DestroyShaderModules(const VkDevice& device);

	VkPipelineVertexInputStateCreateInfo createVertexInputStateInfo();
	VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();
private:
	std::vector<VkPipelineShaderStageCreateInfo> m_VecShadersStageInfos;

	std::string m_VertexShaderFile;
	std::string m_FragmentShaderFile;

	std::array<VkVertexInputAttributeDescription, 2> m_AttributeDescriptions;
	VkVertexInputBindingDescription m_InputBinding;
	VkPipelineShaderStageCreateInfo createFragmentShaderInfo(const VkDevice& device);
	VkPipelineShaderStageCreateInfo createVertexShaderInfo(const VkDevice& device);

	VkShaderModule createShaderModule(const VkDevice& device, const std::vector<char>& code);
};