#pragma once
#include <vulkan/vulkan.h>
#include "GraphicsPipeline.h"

class DescriptorSetManager
{
public:
	DescriptorSetManager() = default;
	~DescriptorSetManager() = default;

	DescriptorSetManager(const DescriptorSetManager& other) = delete;
	DescriptorSetManager(DescriptorSetManager&& other) noexcept = delete;
	DescriptorSetManager& operator=(const DescriptorSetManager& other) = delete;
	DescriptorSetManager& operator=(DescriptorSetManager&& other) noexcept = delete;

	void CreateDescriptorSetLayout(const VkDevice& device,VkDescriptorSetLayout& descriptorSetLayout);

	static VkDescriptorSetLayoutBinding& GetDescriptorSetLayoutBinging()
	{
		return m_UboLayoutBinding;
	}


private:
	 static VkDescriptorSetLayoutBinding m_UboLayoutBinding;
};