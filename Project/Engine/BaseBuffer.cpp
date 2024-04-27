#include "BaseBuffer.h"
#include <stdexcept>
#include "../Engine/CommandPool.h"

void BaseBuffer::CreateBuffer(const VkDeviceSize& size,const  VkBufferUsageFlags& usage,
    const VkMemoryPropertyFlags& properties,VkBuffer& buffer,
    VkDeviceMemory& bufferMemory, const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice,memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void BaseBuffer::CopyBuffer(const VkBuffer& srcBuffer,const VkBuffer& dstBuffer,const VkDeviceSize& size,
    const CommandPool& commandPool, ImageManager& imageManager, const VkDevice& device, const VkQueue& graphicsQueue)
{
    VkCommandBuffer commandBuffer = imageManager.BeginSingleTimeCommands(device, commandPool.GetCommandPool());

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    imageManager.EndSingleTimeCommands(device,commandBuffer, graphicsQueue, commandPool.GetCommandPool());
}

uint32_t BaseBuffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}
