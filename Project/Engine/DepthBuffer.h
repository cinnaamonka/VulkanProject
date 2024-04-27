#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "ImageManager.h"

class DepthBuffer
{

	public:
		DepthBuffer();
		~DepthBuffer() = default;

		DepthBuffer(const DepthBuffer& other) = delete;
		DepthBuffer(DepthBuffer&& other) noexcept = delete;
		DepthBuffer& operator=(const DepthBuffer& other) = delete;
		DepthBuffer& operator=(DepthBuffer&& other) noexcept = delete;

		//create a depth buffer
		void CreateDepthResources(const VkDevice& device, const VkPhysicalDevice& physicalDevice,
			const VkExtent2D& swapChainExtent,ImageManager& imageManager, const VkCommandPool& commandPool, 
			const VkQueue& graphicsQueue);

		const VkFormat& FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling& tiling,
			const VkFormatFeatureFlags& features, const VkPhysicalDevice& physicalDevice);

		const VkFormat& FindDepthFormat(const VkPhysicalDevice& physicalDevice);
		
		static bool HasStencilComponent(const VkFormat& format) {
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		}

		void CleanUp(const VkDevice& device);
		//get the depth image
		VkImage& GetDepthImage()
		{
			return m_DepthImage;
		}
		//get the depth image view
		VkImageView& GetDepthImageView()
		{
			return m_DepthImageView;
		}
		//get the depth image memory
		VkDeviceMemory& GetDepthImageMemory()
		{
			return m_DepthImageMemory;
		}
private:
	VkImage m_DepthImage;
	VkDeviceMemory m_DepthImageMemory;
	VkImageView m_DepthImageView;
	VkFormat m_SupportedFormat;
};