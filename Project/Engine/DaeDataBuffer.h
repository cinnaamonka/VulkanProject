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
	DAEDataBuffer(const DAEDataBuffer& other) 
	{
		m_VkDevice = other.m_VkDevice;
		m_Size = other.m_Size;
		m_VkBuffer = other.m_VkBuffer;
		m_VkBufferMemory = other.m_VkBufferMemory;
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

	static VkDeviceSize& GetDeviceSize()
	{
		return m_Size;
	}

private:

	VkDevice m_VkDevice;
	static VkDeviceSize m_Size;
	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkBufferMemory;
};

