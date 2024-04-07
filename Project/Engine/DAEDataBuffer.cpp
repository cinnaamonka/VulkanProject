#include "DaeDataBuffer.h"
#include <iostream>

VkDeviceSize DAEDataBuffer::m_Size = {};

DAEDataBuffer::DAEDataBuffer(const VkPhysicalDevice& physicalDevice, const VkDevice& device,
	const VkBufferUsageFlags& usage,
	const VkMemoryPropertyFlags& properties, const VkDeviceSize& size)

	:m_VkDevice(device)
{
	CreateBuffer(size, usage, properties, m_VkBuffer, m_VkBufferMemory, device, physicalDevice);
	m_Size = size;
}


void DAEDataBuffer::Upload(const VkDeviceSize& size, void* data)
{
	if (size > m_Size)
	{
		throw std::runtime_error("Upload size exceeds buffer size");
	}

	void* mappedData;
	vkMapMemory(m_VkDevice, m_VkBufferMemory, 0, size, 0, &mappedData);
	memcpy(mappedData, data, static_cast<size_t>(size));
	vkUnmapMemory(m_VkDevice, m_VkBufferMemory);
}

void DAEDataBuffer::Map(const VkDeviceSize& size, void* data)
{
	if (size > m_Size)
	{
		throw std::runtime_error("Map size exceeds buffer size");
	}

	vkMapMemory(m_VkDevice, m_VkBufferMemory, 0, size, 0, &data);
}

void DAEDataBuffer::Destroy()
{
	vkDestroyBuffer(m_VkDevice, m_VkBuffer, nullptr);
	vkFreeMemory(m_VkDevice, m_VkBufferMemory, nullptr);
}

void DAEDataBuffer::BindAsVertexBuffer(const VkCommandBuffer& commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_VkBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}


void DAEDataBuffer::BindAsIndexBuffer(const VkCommandBuffer& commandBuffer)
{
	vkCmdBindIndexBuffer(commandBuffer, m_VkBuffer, 0, VK_INDEX_TYPE_UINT16);
}
void DAEDataBuffer::BindAsModelIndexBuffer(const VkCommandBuffer& commandBuffer)
{
	vkCmdBindIndexBuffer(commandBuffer, m_VkBuffer, 0, VK_INDEX_TYPE_UINT32);
}
VkBuffer& DAEDataBuffer::GetVkBuffer()
{
	return m_VkBuffer;
}

const VkDeviceSize& DAEDataBuffer::GetSizeInBytes()
{
	return m_Size;
}
