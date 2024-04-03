#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <vulkan/vulkan.h>

#include "./Engine/Structs.h"
#include "./Engine/DescriptorPoolManager.h"
#include "../RenderPass.h"

class GP2Shader3D final
{
public:
	GP2Shader3D(const std::string& vertexShaderFile, const std::string& fragmentShaderFile):
		m_VertexShaderFile{ vertexShaderFile },
		m_FragmentShaderFile{ fragmentShaderFile },
		m_VecShadersStageInfos{  },
		m_InputBinding{ Vertex3D::GetBindingDescription() },
		m_AttributeDescriptions{ Vertex3D::GetAttributeDescriptions() }
	{}

	~GP2Shader3D() = default;

	GP2Shader3D(const GP2Shader3D& other) = delete;
	GP2Shader3D(GP2Shader3D&& other) noexcept = delete;
	GP2Shader3D& operator=(const GP2Shader3D& other) = delete;
	GP2Shader3D& operator=(GP2Shader3D&& other) noexcept = delete;

	void Init(const VkDevice& device, const VkPhysicalDevice& physicalDevice,const RenderPass& renderPass,
		const VkExtent2D& swapChainExtent);

	const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStageInfos() { return m_VecShadersStageInfos; }
	void DestroyShaderModules(const VkDevice& device);

	VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
	VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();

	void DestroyDataBuffer();
private:

	std::vector<VkPipelineShaderStageCreateInfo> m_VecShadersStageInfos;

	std::string m_VertexShaderFile;
	std::string m_FragmentShaderFile;

	std::unique_ptr<DAEDataBuffer> m_UBOBuffer;
	VertexUBO m_UBOSrc;

	std::array<VkVertexInputAttributeDescription, 3> m_AttributeDescriptions;
	VkVertexInputBindingDescription m_InputBinding;
	VkPipelineShaderStageCreateInfo createFragmentShaderInfo(const VkDevice& device);
	VkPipelineShaderStageCreateInfo createVertexShaderInfo(const VkDevice& device);

	VkShaderModule CreateShaderModule(const VkDevice& device, const std::vector<char>& code);
};