#include "GP2Shader2D.h"
#include "vulkanbase/VulkanUtil.h"
#include "../Structs.h"
#include <iostream>
#include <memory>


void GP2Shader::Init(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const RenderPass& renderPass,
	const VkExtent2D& swapChainExtent)
{
	m_VecShadersStageInfos.push_back(createFragmentShaderInfo(device));
	m_VecShadersStageInfos.push_back(createVertexShaderInfo(device));

	m_DescriptorPool = std::make_unique<DAEDescriptorPool<ViewProjection>>(device, 1);

	m_DescriptorPool->initialize(VulkanContext{ device,physicalDevice,renderPass.GetRenderPass(), swapChainExtent}, physicalDevice, device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(VertexUBO));

	m_DescriptorPool->CreateDescriptorSetLayout(VulkanContext{ device,physicalDevice,renderPass.GetRenderPass(), swapChainExtent });

	m_UBOBuffer = std::make_unique<DAEDataBuffer>(
		physicalDevice,
		device,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		sizeof(VertexUBO)
	);
}
void GP2Shader::DestroyShaderModules(const VkDevice& device)
{
	for (VkPipelineShaderStageCreateInfo& stageInfo : m_VecShadersStageInfos)
	{
		vkDestroyShaderModule(device, stageInfo.module, nullptr);
	}
	m_VecShadersStageInfos.clear();
}
VkPipelineShaderStageCreateInfo GP2Shader::createFragmentShaderInfo(const VkDevice& device)
{
	std::vector<char> fragShaderCode = readFile(m_FragmentShaderFile);
	VkShaderModule fragShaderModule = createShaderModule(device, fragShaderCode);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	return fragShaderStageInfo;
}

VkPipelineShaderStageCreateInfo GP2Shader::createVertexShaderInfo(const VkDevice& device)
{
	std::vector<char> vertShaderCode = readFile(m_VertexShaderFile);
	VkShaderModule vertShaderModule = createShaderModule(device, vertShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	return vertShaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo GP2Shader::createVertexInputStateInfo()
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{ };

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_AttributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &m_InputBinding;
	vertexInputInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data();
	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo GP2Shader::createInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkShaderModule GP2Shader::createShaderModule(const VkDevice& device, const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}