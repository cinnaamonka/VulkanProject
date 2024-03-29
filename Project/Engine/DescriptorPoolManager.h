#pragma once
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <vector>
#include <memory>
#include "GPUObject.h"

template <class UBO>
class DAEDescriptorPool
{
public:
	DAEDescriptorPool(const VkDevice& device, size_t count);
	~DAEDescriptorPool();

	void initialize(const VulkanContext& context, const VkPhysicalDevice& physicalDevice,
		const VkDevice& device,
		const VkBufferUsageFlags& usage,
		const VkMemoryPropertyFlags& properties,
		const VkDeviceSize& size);

	void setUBO(UBO data, size_t index);

	VkDescriptorSetLayout& getDescriptorSetLayout()
	{
		return m_DescriptorSetLayout;
	}

	void createDescriptorSets();

	void bindDescriptorSet(VkCommandBuffer buffer, VkPipelineLayout layout, size_t index);

	void CreateDescriptorSetLayout(const VulkanContext& context);

private:
	VkDevice m_Device;
	VkDeviceSize m_Size;
	VkDescriptorSetLayout m_DescriptorSetLayout;

	
	void createUBOs(const VulkanContext& context, const VkPhysicalDevice& physicalDevice,
		const VkDevice& device,
		const VkBufferUsageFlags& usage,
		const VkMemoryPropertyFlags& properties,
		const VkDeviceSize& size);

	VkDescriptorPool m_DescriptorPool;
	std::vector<VkDescriptorSet> m_DescriptorSets;
	std::vector<DAEUniformBufferObject<UBO>> m_UBOs;

	size_t m_Count;
};

template <class UBO>
DAEDescriptorPool<UBO>::DAEDescriptorPool(const VkDevice& device, size_t count)

	:m_Device{ device },
	m_Size{ sizeof(UBO) },
	m_Count(count),
	m_DescriptorPool{ nullptr },
	m_DescriptorSetLayout{ nullptr }
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(count);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(count);

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

template <class UBO>
DAEDescriptorPool<UBO>::~DAEDescriptorPool()
{
	for (DAEUniformBufferObject<UBO>& buffer : m_UBOs)
	{
		buffer.reset();
	}
	//vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);
	//vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
}

template<class UBO>
inline void DAEDescriptorPool<UBO>::initialize(const VulkanContext& context, const VkPhysicalDevice& physicalDevice,
	const VkDevice& device,
	const VkBufferUsageFlags& usage,
	const VkMemoryPropertyFlags& properties,
	const VkDeviceSize& size)
{
	CreateDescriptorSetLayout(context);
	createUBOs(context,physicalDevice,device,usage,properties,size);
	createDescriptorSets();
}

template <class UBO>
void DAEDescriptorPool<UBO>::createDescriptorSets()
{

	std::vector<VkDescriptorSetLayout> layouts(m_Count, m_DescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_DescriptorPool;
	allocInfo.descriptorSetCount = m_Count;
	allocInfo.pSetLayouts = layouts.data();

	m_DescriptorSets.resize(m_Count);
	if (vkAllocateDescriptorSets(m_Device, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}


	size_t descriptorIndex = 0;
	for (DAEUniformBufferObject<UBO>& buffer : m_UBOs)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = buffer.getVkBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = m_Size;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[descriptorIndex];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(m_Device, 1, &descriptorWrite, 0, nullptr);
		++descriptorIndex;
	}

}

template <class UBO>
void DAEDescriptorPool<UBO>::bindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, size_t index)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorSets[index], 0, nullptr);
}

template<class UBO>
inline void DAEDescriptorPool<UBO>::CreateDescriptorSetLayout(const VulkanContext& context)
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;

	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(context.device, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
 {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

template<class UBO>
inline void DAEDescriptorPool<UBO>::createUBOs(const VulkanContext& context, const VkPhysicalDevice& physicalDevice,
	const VkDevice& device,
	const VkBufferUsageFlags& usage,
	const VkMemoryPropertyFlags& properties,
	const VkDeviceSize& size)
{
	for (int uboIndex = 0; uboIndex < m_Count; ++uboIndex)
	{
		auto buffer = DAEUniformBufferObject<UBO>{};
		buffer.initialize(context,physicalDevice,device,usage,properties,size);
		m_UBOs.emplace_back(std::move(buffer));
	}
}

template<class UBO>
inline void DAEDescriptorPool<UBO>::setUBO(UBO src, size_t index)
{
	if (index < m_UBOs.size())
	{
		m_UBOs[index].setData(src);
		m_UBOs[index].upload();
	}
}


