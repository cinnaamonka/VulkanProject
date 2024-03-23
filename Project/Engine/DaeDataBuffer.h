#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>

#include "BaseBuffer.h"

class DAEDataBuffer : public BaseBuffer
{
public:
	DAEDataBuffer(
		VkPhysicalDevice physicalDevice,
		VkDevice device,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkDeviceSize size
	);

	~DAEDataBuffer() 
	{
		Destroy();
	}

	void Upload(VkDeviceSize size, void* data);
	void Map(VkDeviceSize size, void* data);
	void Destroy();
	void BindAsVertexBuffer(VkCommandBuffer commandBuffer);
	void BindAsIndexBuffer(VkCommandBuffer commandBuffer);
	VkBuffer GetVkBuffer();
	VkDeviceSize GetSizeInBytes();
private:

	VkDevice m_VkDevice;
	VkDeviceSize m_Size;
	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkBufferMemory;
};

