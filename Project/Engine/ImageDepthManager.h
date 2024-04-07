#pragma once

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class ImageDepthManager
{
public:
	ImageDepthManager() = default;
	~ImageDepthManager() = default;

	ImageDepthManager(const ImageDepthManager& other) = delete;
	ImageDepthManager& operator=(const ImageDepthManager& other) = delete;
	ImageDepthManager(ImageDepthManager&& other) = delete;
	ImageDepthManager& operator=(ImageDepthManager&& other) = delete;

	VkFormat& FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling& tiling,const VkFormatFeatureFlags& features,
		const VkPhysicalDevice& physicalDevice);

	void CreateDepthResources(const VkPhysicalDevice& physicalDevice);

	VkFormat& FindDepthFormat(const VkPhysicalDevice& physicalDevice);
private:
	bool HasStencilComponent(const VkFormat& format);

private:
	VkImage m_DepthImage; 
	VkDeviceMemory m_DepthImageMemory; 
	VkImageView m_DepthImageView; 
};