#include "VertexBuffer.h"

void VertexBuffer::CreateVertexBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
	CreateBuffer(size, usage, properties, buffer, bufferMemory, device, physicalDevice);
}
