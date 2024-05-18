#define STB_IMAGE_IMPLEMENTATION
#include <../image_stb-src/include/stb/stb_image.h>

#include "ImageManager.h"
#include "BaseBuffer.h"
#include "DepthBuffer.h"
#include <stdexcept>
#include <filesystem>

ImageManager::ImageManager()
{
}

void ImageManager::CreateTextureImage(const VkDevice& device, const VkPhysicalDevice& physicalDevice,
	const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const std::string& diffuseTexturePath,
	const std::string& normalPath, const std::string& specularPath, const std::string& roughnessPath)
{
	m_DiffuseTexture.CreateTextureImage(device, physicalDevice, commandPool, graphicsQueue, diffuseTexturePath);
	m_NormalMap.CreateTextureImage(device, physicalDevice, commandPool, graphicsQueue, normalPath);
	m_SpecularMap.CreateTextureImage(device, physicalDevice, commandPool, graphicsQueue, specularPath);
	m_RoughnessMap.CreateTextureImage(device, physicalDevice, commandPool, graphicsQueue, roughnessPath);
}

void ImageManager::CreateImage(const VkDevice& device, uint32_t width, uint32_t height, const VkFormat& format,
	const VkImageTiling& tiling, const VkImageUsageFlags& usage, const VkMemoryPropertyFlags& properties,
	VkImage& image, VkDeviceMemory& imageMemory, const VkPhysicalDevice& physicalDevice)
{
	m_DiffuseTexture.CreateImage(device, width, height, format, tiling, usage, properties, image, imageMemory, physicalDevice);
}

VkCommandBuffer& ImageManager::BeginSingleTimeCommands(const VkDevice& device, const VkCommandPool& commandPool)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers(device, &allocInfo, &m_CommandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);

	return m_CommandBuffer;
}


void ImageManager::EndSingleTimeCommands(const VkDevice& device, const VkCommandBuffer& commandBuffer,
	const VkQueue& graphicsQueue, const VkCommandPool& commandPool)
{

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
} 

void ImageManager::TransitionImageLayout(const VkDevice& device, const VkCommandPool& commandPool,
	const VkQueue& graphicsQueue, const VkImage& image,const VkFormat& format,const VkImageLayout& oldLayout,const VkImageLayout& newLayout)
{
	m_DiffuseTexture.TransitionImageLayout(device, commandPool, graphicsQueue, image, format, oldLayout, newLayout); 
	m_NormalMap.TransitionImageLayout(device, commandPool, graphicsQueue, image, format, oldLayout, newLayout);
	m_SpecularMap.TransitionImageLayout(device, commandPool, graphicsQueue, image, format, oldLayout, newLayout);
	m_RoughnessMap.TransitionImageLayout(device, commandPool, graphicsQueue, image, format, oldLayout, newLayout);
}

void ImageManager::CopyBufferToImage(const VkBuffer& buffer, const VkImage& image, uint32_t width,
	uint32_t height, const VkCommandPool& commandPool, const VkDevice& device, const VkQueue& graphicsQueue)
{
	m_DiffuseTexture.CopyBufferToImage(buffer, image, width, height, commandPool, device, graphicsQueue);
	m_NormalMap.CopyBufferToImage(buffer, image, width, height, commandPool, device, graphicsQueue);
	m_SpecularMap.CopyBufferToImage(buffer, image, width, height, commandPool, device, graphicsQueue);
	m_RoughnessMap.CopyBufferToImage(buffer, image, width, height, commandPool, device, graphicsQueue);
}

void ImageManager::CleanUp(const VkDevice& device)
{
	m_DiffuseTexture.CleanUp(device);
	m_NormalMap.CleanUpWithoutImageViews(device);
	m_SpecularMap.CleanUpWithoutImageViews(device);
	m_RoughnessMap.CleanUpWithoutImageViews(device);
}

void ImageManager::CreateTextureImageView(const VkDevice& device)
{
	m_DiffuseTexture.CreateTextureImageView(device);
	m_NormalMap.CreateTextureImageView(device);
	m_SpecularMap.CreateTextureImageView(device);
	m_RoughnessMap.CreateTextureImageView(device);
}

VkImageView& ImageManager::CreateImageView(const VkImage& image, const VkFormat& format, const VkDevice& device,
	const VkImageAspectFlags& aspectFlags)
{
	return m_DiffuseTexture.CreateImageView(image, format, device, aspectFlags);
}

void ImageManager::CreateDiffuseTextureSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
	return m_DiffuseTexture.CreateTextureSampler(device, physicalDevice); 
}
void ImageManager::CreateNormalMapSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
	return m_NormalMap.CreateTextureSampler(device, physicalDevice);
}

void ImageManager::CreateSpecularMapSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
	return m_SpecularMap.CreateTextureSampler(device, physicalDevice);
}

void ImageManager::CreateRoughnessMapSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
	return m_RoughnessMap.CreateTextureSampler(device, physicalDevice);
}

