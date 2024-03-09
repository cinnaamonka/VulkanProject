#include "vulkanbase/VulkanUtil.h"
#include "CommandPool.h"
#include "Structs.h"
#include "CommandBuffer.h"


void CommandPool::CreateCommandPool(const VkDevice& device, const QueueFamilyIndices& familyIndices)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = familyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void CommandPool::DestroyCommandPool(const VkDevice& device)
{
	if (m_CommandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device, m_CommandPool, nullptr);
		m_CommandPool = VK_NULL_HANDLE;
	}
}

VulkanCommandBuffer CommandPool::CreateCommandBuffer(const VkDevice& device) const
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer buffer{};

	if (vkAllocateCommandBuffers(device, &allocInfo, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	VulkanCommandBuffer commandBuffer{};
	commandBuffer.SetVKCommandBuffer(buffer);
	return commandBuffer;
}