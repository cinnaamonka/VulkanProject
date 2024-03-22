#pragma once
#include "BaseBuffer.h"

#include <vector>

class IndexBuffer:public BaseBuffer
{
public:
	IndexBuffer() = default;
	~IndexBuffer() = default;

	IndexBuffer(const IndexBuffer& other) = delete;
	IndexBuffer& operator=(const IndexBuffer& other) = delete;
	IndexBuffer(IndexBuffer&& other) = delete;
	IndexBuffer& operator=(IndexBuffer&& other) = delete;

	static void CreateIndexBuffer(const std::vector<uint16_t> indices, const VkDevice& device, const CommandPool& commandPool,
		const VkQueue& graphicsQueue, const VkPhysicalDevice& physicalDevice,VkBuffer& indexBuffer,VkDeviceMemory& deviceMemory);

	static void DestroyIndexBuffer(const VkDevice& device,const VkBuffer& indexBuffer, VkDeviceMemory& deviceMemory);

	//static VkBuffer& GetIndexBuffer()
	//{
	//	return indexBuffer;
	//}

	//static VkDeviceMemory& GetIndexBufferMemory()
	//{
	//	return indexBufferMemory;
	//}

private:
	//static VkBuffer indexBuffer;
	//static VkDeviceMemory indexBufferMemory;
};