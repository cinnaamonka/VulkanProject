#pragma once
#include <vulkan/vulkan.h>
#include <string>

class Texture
{
public:

	Texture() = default;
	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) = delete;
	~Texture() = default;

	void CreateTextureImage(const VkDevice& device, const VkPhysicalDevice& physicalDevice,
		const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const std::string& imagePath);

	void CreateImage(const VkDevice& device, uint32_t width, uint32_t height, const VkFormat& format,
		const VkImageTiling& tiling, const VkImageUsageFlags& usage, const VkMemoryPropertyFlags& properties, VkImage& image,
		VkDeviceMemory& imageMemory, const VkPhysicalDevice& physicalDevice);

	VkCommandBuffer& BeginSingleTimeCommands(const VkDevice& device, const VkCommandPool& commandPool);
	void EndSingleTimeCommands(const VkDevice& device, const VkCommandBuffer& commandBuffer, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);

	void TransitionImageLayout(const VkDevice& device, const VkCommandPool& commandPool,
		const VkQueue& graphicsQueue, const VkImage& image,const VkFormat& format,const VkImageLayout& oldLayout,const VkImageLayout& newLayout);

	void CopyBufferToImage(const VkBuffer& buffer, const VkImage& image, uint32_t width, uint32_t height,
		const VkCommandPool& commandPool, const VkDevice& device, const VkQueue& graphicsQueue);

	void CleanUp(const VkDevice& device);

	void CreateTextureImageView(const VkDevice& device);

	VkImageView& CreateImageView(const VkImage& image, const VkFormat& format, const VkDevice& device, const VkImageAspectFlags& aspectFlags);

	void CreateTextureSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice);


	VkBuffer& GetStagingBuffer() 
	{
		return m_StagingBuffer;
	}

	VkDeviceMemory& GetStagingBufferMemory() 
	{
		return m_StagingBufferMemory;
	}

	VkImage& GetTextureImage() 
	{
		return m_TextureImage;
	}

	VkDeviceMemory& GetTextureImageMemory() 
	{
		return m_TextureImageMemory;
	}

	VkPipelineStageFlags& GetSourceStage()
	{
		return m_SourceStage;
	}

	VkPipelineStageFlags& GetDestinationStage() 
	{
		return m_DestinationStage;
	}

	VkImageView& GetTextureImageView() 
	{
		return m_TextureImageView;
	}

	VkPhysicalDeviceProperties& GetProperties() 
	{
		return m_Properties;
	}

	VkSampler& GetTextureSampler() 
	{
		return m_TextureSampler;
	}

	VkImageView& GetImageView() 
	{
		return m_ImageView;
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
	VkImageView m_ImageView;

	VkCommandBuffer m_TempCommandBuffer;
};