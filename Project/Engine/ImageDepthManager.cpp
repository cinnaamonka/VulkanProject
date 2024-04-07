#include "ImageDepthManager.h"
#include "stdexcept"

VkFormat& ImageDepthManager::FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling& tiling,
    const VkFormatFeatureFlags& features, const VkPhysicalDevice& physicalDevice)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

void ImageDepthManager::CreateDepthResources(const VkPhysicalDevice& physicalDevice)
{
    VkFormat depthFormat = FindDepthFormat(physicalDevice);

}

VkFormat& ImageDepthManager::FindDepthFormat(const VkPhysicalDevice& physicalDevice)
{
    return FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, physicalDevice
    );
}

bool ImageDepthManager::HasStencilComponent(const VkFormat& format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

