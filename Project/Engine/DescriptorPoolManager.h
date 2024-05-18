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

	void initialize(const VkPhysicalDevice& physicalDevice,
		const VkDevice& device,
		const VkBufferUsageFlags& usage,
		const VkMemoryPropertyFlags& properties,
		const VkDeviceSize& size, ImageManager& imageMamager);

	void setUBO(UBO data, size_t index);

	VkDescriptorSetLayout& getDescriptorSetLayout()
	{
		return m_DescriptorSetLayout;
	}

	void createDescriptorSets(ImageManager& imageManager);

	void bindDescriptorSet(VkCommandBuffer buffer, VkPipelineLayout layout, size_t index);

	void CreateDescriptorSetLayout(const VkDevice& device);

	void DestroyDescriptorPool()
	{
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
		DestroyUBOs();
	}

	void DestroyUBOs()
	{
		for (DAEUniformBufferObject<UBO>& ubo : m_UBOs)
		{
			ubo.DestroyGPUObject();
		}
	}
private:
	VkDevice m_Device;
	VkDeviceSize m_Size;
	VkDescriptorSetLayout m_DescriptorSetLayout;


	void createUBOs(const VkPhysicalDevice& physicalDevice,
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
	m_DescriptorSetLayout{ nullptr },
	m_DescriptorPool{}
{

}

template <class UBO>
DAEDescriptorPool<UBO>::~DAEDescriptorPool()
{
	for (DAEUniformBufferObject<UBO>& buffer : m_UBOs)
	{
		buffer.reset();
	}
}

template<class UBO>
inline void DAEDescriptorPool<UBO>::initialize(const VkPhysicalDevice& physicalDevice,
	const VkDevice& device,
	const VkBufferUsageFlags& usage,
	const VkMemoryPropertyFlags& properties,
	const VkDeviceSize& size,ImageManager& imageMamager)
{
	std::array<VkDescriptorPoolSize, 5> poolSizes{}; 
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
	poolSizes[0].descriptorCount = static_cast<uint32_t>(m_Count);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
	poolSizes[1].descriptorCount = static_cast<uint32_t>(m_Count);
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[2].descriptorCount = static_cast<uint32_t>(m_Count);
	poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[3].descriptorCount = static_cast<uint32_t>(m_Count);
	poolSizes[4].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[4].descriptorCount = static_cast<uint32_t>(m_Count);

	VkDescriptorPoolCreateInfo poolInfo{}; 
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; 
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size()); 
	poolInfo.pPoolSizes = poolSizes.data(); 
	poolInfo.maxSets = static_cast<uint32_t>(m_Count);

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

	CreateDescriptorSetLayout(device);
	createUBOs(physicalDevice, device, usage, properties, size);
	createDescriptorSets(imageMamager);
}

template <class UBO>
void DAEDescriptorPool<UBO>::createDescriptorSets(ImageManager& imageManager)
{

	std::vector<VkDescriptorSetLayout> layouts(m_Count, m_DescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_DescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(m_Count);
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

		VkDescriptorImageInfo diffuseImageInfo{}; 
		diffuseImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; 
		diffuseImageInfo.imageView = imageManager.GetDiffuseTextureImageView();
		diffuseImageInfo.sampler = imageManager.GetDiffuseTextureSampler(); 

		VkDescriptorImageInfo normalMapInfo{};
		normalMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		normalMapInfo.imageView = imageManager.GetNormalMapImageView();
		normalMapInfo.sampler = imageManager.GetNormalMapTextureSampler();

		VkDescriptorImageInfo specularMapInfo{};
		specularMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		specularMapInfo.imageView = imageManager.GetSpecularMapImageView();
		specularMapInfo.sampler = imageManager.GetSpecularMapTextureSampler(); 

		VkDescriptorImageInfo roughnessMapInfo{};
		roughnessMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		roughnessMapInfo.imageView = imageManager.GetRoughnessMapImageView(); 
		roughnessMapInfo.sampler = imageManager.GetRoughnessMapTextureSampler(); 

		std::array<VkWriteDescriptorSet, 5> descriptorWrites{}; 

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_DescriptorSets[descriptorIndex];
		descriptorWrites[0].dstBinding = 0; 
		descriptorWrites[0].dstArrayElement = 0; 
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
		descriptorWrites[0].descriptorCount = 1; 
		descriptorWrites[0].pBufferInfo = &bufferInfo; 


		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; 
		descriptorWrites[1].dstSet = m_DescriptorSets[descriptorIndex];
		descriptorWrites[1].dstBinding = 1; 
		descriptorWrites[1].dstArrayElement = 0; 
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
		descriptorWrites[1].descriptorCount = 1; 
		descriptorWrites[1].pImageInfo = &diffuseImageInfo;

		// Binding 2: Combined image sampler (second texture)
		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = m_DescriptorSets[descriptorIndex];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pImageInfo = &normalMapInfo;

		descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[3].dstSet = m_DescriptorSets[descriptorIndex];
		descriptorWrites[3].dstBinding = 3;
		descriptorWrites[3].dstArrayElement = 0;
		descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[3].descriptorCount = 1;
		descriptorWrites[3].pImageInfo = &specularMapInfo;

		descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[4].dstSet = m_DescriptorSets[descriptorIndex];
		descriptorWrites[4].dstBinding = 4;
		descriptorWrites[4].dstArrayElement = 0;
		descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[4].descriptorCount = 1;
		descriptorWrites[4].pImageInfo = &roughnessMapInfo;

		
		vkUpdateDescriptorSets(m_Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		++descriptorIndex;
	}

}

template <class UBO>
void DAEDescriptorPool<UBO>::bindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, size_t index)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorSets[index], 0, nullptr);
}

template<class UBO>
inline void DAEDescriptorPool<UBO>::CreateDescriptorSetLayout(const VkDevice& device)
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

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding secondSamplerLayoutBinding{};
	secondSamplerLayoutBinding.binding = 2;
	secondSamplerLayoutBinding.descriptorCount = 1;
	secondSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	secondSamplerLayoutBinding.pImmutableSamplers = nullptr;
	secondSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding thirdSamplerLayoutBinding{};
	thirdSamplerLayoutBinding.binding = 3;
	thirdSamplerLayoutBinding.descriptorCount = 1;
	thirdSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	thirdSamplerLayoutBinding.pImmutableSamplers = nullptr;
	thirdSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding fourthSamplerLayoutBinding{};
	fourthSamplerLayoutBinding.binding = 4;
	fourthSamplerLayoutBinding.descriptorCount = 1;
	fourthSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	fourthSamplerLayoutBinding.pImmutableSamplers = nullptr;
	fourthSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 5> bindings = { uboLayoutBinding, samplerLayoutBinding,secondSamplerLayoutBinding,thirdSamplerLayoutBinding,fourthSamplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfoSampler{};
	layoutInfoSampler.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfoSampler.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfoSampler.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device, &layoutInfoSampler, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}


template<class UBO>
inline void DAEDescriptorPool<UBO>::createUBOs(const VkPhysicalDevice& physicalDevice,
	const VkDevice& device,
	const VkBufferUsageFlags& usage,
	const VkMemoryPropertyFlags& properties,
	const VkDeviceSize& size)
{
	for (int uboIndex = 0; uboIndex < m_Count; ++uboIndex)
	{
		auto buffer = DAEUniformBufferObject<UBO>{};
		buffer.initialize(physicalDevice, device, usage, properties, size);
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


