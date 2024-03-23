#include "DaeDataBuffer.h"

DAEDataBuffer::DAEDataBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkBufferUsageFlags usage, 
	VkMemoryPropertyFlags properties, VkDeviceSize size)
	:m_VkDevice(device), m_Size(size)
{
	CreateBuffer(size,usage, properties, m_VkBuffer, m_VkBufferMemory,device,physicalDevice);
}


void DAEDataBuffer::Upload(VkDeviceSize size, void* data)
{
	if (size > m_Size)
	{
		throw std::runtime_error("Upload size exceeds buffer size");
	}

	void* mappedData;
	vkMapMemory(m_VkDevice, m_VkBufferMemory, 0, size, 0, &mappedData);
	memcpy(mappedData, data, static_cast<size_t>(size));
}

void DAEDataBuffer::Map(VkDeviceSize size, void* data)
{
	if (size > m_Size)
	{
		throw std::runtime_error("Map size exceeds buffer size");
	}

	vkMapMemory(m_VkDevice, m_VkBufferMemory, 0, size, 0, &data);
}

void DAEDataBuffer::Destroy()
{
	if (m_VkBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_VkDevice, m_VkBuffer, nullptr);
		m_VkBuffer = VK_NULL_HANDLE;
	}

	if (m_VkBufferMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_VkDevice, m_VkBufferMemory, nullptr);
		m_VkBufferMemory = VK_NULL_HANDLE;
	}
}

void DAEDataBuffer::BindAsVertexBuffer(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_VkBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}


void DAEDataBuffer::BindAsIndexBuffer(VkCommandBuffer commandBuffer)
{
	vkCmdBindIndexBuffer(commandBuffer, m_VkBuffer, 0, VK_INDEX_TYPE_UINT16);
}

VkBuffer DAEDataBuffer::GetVkBuffer()
{
	return m_VkBuffer;
}

VkDeviceSize DAEDataBuffer::GetSizeInBytes()
{
	return m_Size;
}
