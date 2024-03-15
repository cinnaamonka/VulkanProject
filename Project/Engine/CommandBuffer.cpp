#include "CommandBuffer.h"
#include "CommandPool.h"

void VulkanCommandBuffer::ResetCommandBuffer()const
{
	vkResetCommandBuffer(m_CommandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
}
void VulkanCommandBuffer::SubmitCommandBuffer(VkSubmitInfo& info)const
{
	info.commandBufferCount = 1; 
	info.pCommandBuffers = &m_CommandBuffer; 
}


void VulkanCommandBuffer::RecordCommandBuffer()
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; 
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

}
void VulkanCommandBuffer::EndRecordCommandBuffer()
{
	if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}
