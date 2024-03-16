#pragma once
#include "BaseBuffer.h"

class VertexBuffer: public BaseBuffer
{
public:
    VertexBuffer() = default;
    ~VertexBuffer() = default;

    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept = delete;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept = delete;

    static void CreateVertexBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
        VkBuffer& buffer, VkDeviceMemory& bufferMemory,
        const VkDevice& device, const VkPhysicalDevice& physicalDevice);
};