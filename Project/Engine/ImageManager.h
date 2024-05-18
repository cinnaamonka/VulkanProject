#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Texture.h"
class ImageManager
{
public:
	ImageManager();
	~ImageManager() = default;

	void CreateTextureImage(const VkDevice& device,const VkPhysicalDevice& physicalDevice, 
		const VkCommandPool& commandPool, const VkQueue& graphicsQueue,const std::string& diffuseTexturePath,
		const std::string& normalPathPath, const std::string& normalPath, const std::string& roughnessPath);

	void CreateImage(const VkDevice& device, uint32_t width, uint32_t height,const VkFormat& format,
		const VkImageTiling& tiling,const VkImageUsageFlags& usage,const VkMemoryPropertyFlags& properties, VkImage& image,
		VkDeviceMemory& imageMemory, const VkPhysicalDevice& physicalDevice);
	 
	VkCommandBuffer& BeginSingleTimeCommands(const VkDevice& device,const VkCommandPool& commandPool);
	void EndSingleTimeCommands(const VkDevice& device,const VkCommandBuffer& commandBuffer,const VkQueue& graphicsQueue,const VkCommandPool& commandPool);
	
	void TransitionImageLayout(const VkDevice& device,const VkCommandPool& commandPool,
		const VkQueue& graphicsQueue,const VkImage& image, const VkFormat& format, const VkImageLayout& oldLayout, const VkImageLayout& newLayout);

	void CopyBufferToImage(const VkBuffer& buffer,const VkImage& image, uint32_t width, uint32_t height,
		const VkCommandPool& commandPool, const VkDevice& device, const VkQueue& graphicsQueue);

	void CleanUp(const VkDevice& device);

	void CreateTextureImageView(const VkDevice& device);

	VkImageView& CreateImageView(const VkImage& image,const VkFormat& format, const VkDevice& device, const VkImageAspectFlags& aspectFlags);

	void CreateDiffuseTextureSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
	void CreateNormalMapSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
	void CreateSpecularMapSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
	void CreateRoughnessMapSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
		
	VkImageView& GetDiffuseTextureImageView()
	{
		return  m_DiffuseTexture.GetTextureImageView();;
	}
	
	VkSampler& GetDiffuseTextureSampler()
	{
		return m_DiffuseTexture.GetTextureSampler();;
	}


	VkImageView& GetNormalMapImageView()
	{
		return  m_NormalMap.GetTextureImageView();;
	}

	VkSampler& GetNormalMapTextureSampler()
	{
		return m_NormalMap.GetTextureSampler();;
	}

	VkImageView& GetSpecularMapImageView()
	{
		return  m_SpecularMap.GetTextureImageView();;
	}

	VkSampler& GetSpecularMapTextureSampler()
	{
		return m_SpecularMap.GetTextureSampler();;
	}

	VkImageView& GetRoughnessMapImageView()
	{
		return  m_RoughnessMap.GetTextureImageView();;
	}

	VkSampler& GetRoughnessMapTextureSampler()
	{
		return m_RoughnessMap.GetTextureSampler();;
	}
private:
	Texture m_DiffuseTexture;
	Texture m_NormalMap;
	Texture m_SpecularMap;
	Texture m_RoughnessMap;
	
	VkCommandBuffer m_CommandBuffer;
};