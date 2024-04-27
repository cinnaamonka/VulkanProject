#include "DepthBuffer.h"
#include <stdexcept>

DepthBuffer::DepthBuffer():
    m_DepthImage(VK_NULL_HANDLE)
{
}

void DepthBuffer::CreateDepthResources(const VkDevice& device, const VkPhysicalDevice& physicalDevice,
    const VkExtent2D& swapChainExtent,ImageManager& imageManager,const VkCommandPool& commandPool,
    const VkQueue& graphicsQueue)
{
    const VkFormat& depthFormat = FindDepthFormat(physicalDevice);

    imageManager.CreateImage(device,swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        m_DepthImage, m_DepthImageMemory,
        physicalDevice);

    m_DepthImageView = imageManager.CreateImageView(m_DepthImage, depthFormat,device, VK_IMAGE_ASPECT_DEPTH_BIT);

    imageManager.TransitionImageLayout(device, commandPool, graphicsQueue, m_DepthImage,
        depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

const VkFormat& DepthBuffer::FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling& tiling, 
    const VkFormatFeatureFlags& features, const VkPhysicalDevice& physicalDevice)
{
    for (const VkFormat& format : candidates)
    {
        VkFormatProperties props;
        m_SupportedFormat = format;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, m_SupportedFormat, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
        {
            return m_SupportedFormat;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return m_SupportedFormat;
        }

        throw std::runtime_error("failed to find supported format!");
    }


}

const VkFormat& DepthBuffer::FindDepthFormat(const VkPhysicalDevice& physicalDevice)
{
    return FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
        physicalDevice
    );
}

void DepthBuffer::CleanUp(const VkDevice& device)
{
	vkDestroyImage(device, m_DepthImage, nullptr);
	vkFreeMemory(device, m_DepthImageMemory, nullptr);
}
