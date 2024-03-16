#pragma once
#include <vulkan/vulkan.h>

class CommandPool;

class BaseBuffer
{
public:
	BaseBuffer() = default;
	virtual ~BaseBuffer() = default;

    BaseBuffer(const BaseBuffer& other) = delete;
    BaseBuffer(BaseBuffer&& other) noexcept = delete;
    BaseBuffer& operator=(const BaseBuffer& other) = delete;
    BaseBuffer& operator=(BaseBuffer&& other) noexcept = delete;

    static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                       VkBuffer& buffer,VkDeviceMemory& bufferMemory,
                      const VkDevice& device, const VkPhysicalDevice& physicalDevice);

    static void CopyBuffer(const VkBuffer& srcBuffer,const VkBuffer& dstBuffer,const VkDeviceSize& size,const VkDevice& device,
        const CommandPool& commandPool,const VkQueue& graphicsQueue);
private:

    static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice,
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties);
};
