#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>

#include "BaseBuffer.h"

class DAEDataBuffer : public BaseBuffer
{
public:
	DAEDataBuffer(
		const VkPhysicalDevice& physicalDevice,
		const VkDevice& device,
		const VkBufferUsageFlags& usage,
		const VkMemoryPropertyFlags& properties,
		const VkDeviceSize& size
	);

	~DAEDataBuffer() 
	{
		Destroy();
	}

	void Upload(const VkDeviceSize& size, void* data);
	void Map(const VkDeviceSize& size, void* data);
	void Destroy();
	void BindAsVertexBuffer(const VkCommandBuffer& commandBuffer);
	void BindAsIndexBuffer(const VkCommandBuffer& commandBuffer);
	VkBuffer& GetVkBuffer();
	const VkDeviceSize& GetSizeInBytes();

	VkDeviceMemory& GetDeviceMemory()
	{
		return m_VkBufferMemory;
	}

private:

	VkDevice m_VkDevice;
	VkDeviceSize m_Size;
	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkBufferMemory;
};

