#pragma once
#include <vulkan/vulkan.h>
#include <string>
class ImageManager
{
public:
	ImageManager() = default;
	~ImageManager() = default;

	

	void CreateTextureImage(const VkDevice& device,const VkPhysicalDevice& physicalDevice, 
		const VkCommandPool& commandPool, const VkQueue& graphicsQueue,const std::string& imagePath);

	void CreateImage(const VkDevice& device, uint32_t width, uint32_t height,const VkFormat& format,
		const VkImageTiling& tiling,const VkImageUsageFlags& usage,const VkMemoryPropertyFlags& properties, VkImage& image,
		VkDeviceMemory& imageMemory, const VkPhysicalDevice& physicalDevice);
	 
	VkCommandBuffer BeginSingleTimeCommands(const VkDevice& device,const VkCommandPool& commandPool);
	void EndSingleTimeCommands(const VkDevice& device,const VkCommandBuffer& commandBuffer,const VkQueue& graphicsQueue,const VkCommandPool& commandPool);
	
	void TransitionImageLayout(const VkDevice& device,const VkCommandPool& commandPool,
		const VkQueue& graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void CopyBufferToImage(const VkBuffer& buffer,const VkImage& image, uint32_t width, uint32_t height,
		const VkCommandPool& commandPool, const VkDevice& device, const VkQueue& graphicsQueue);

	void CleanUp(const VkDevice& device);

	void CreateTextureImageView(const VkDevice& device);

	VkImageView& CreateImageView(const VkImage& image,const VkFormat& format, const VkDevice& device);

	void CreateTextureSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
		
	VkImageView& GetTextureImageView()
	{
		return m_TextureImageView;
	}
	
	VkSampler& GetTextureSampler()
	{
		return m_TextureSampler;
	}
private:
	VkBuffer m_StagingBuffer;
	VkDeviceMemory m_StagingBufferMemory;
	VkImage m_TextureImage;
	VkDeviceMemory m_TextureImageMemory;
	VkPipelineStageFlags m_SourceStage;
	VkPipelineStageFlags m_DestinationStage;
	VkImageView m_TextureImageView;
	VkPhysicalDeviceProperties m_Properties;
	VkSampler m_TextureSampler;
};