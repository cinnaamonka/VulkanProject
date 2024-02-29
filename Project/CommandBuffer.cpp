#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Renderer.h"

VulkanCommandBuffer::VulkanCommandBuffer()
{
   //std::function<void(uint32_t, VkCommandBuffer)> drawFrameFunction,const VkDevice& device,const VkCommandPool& commandPool
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    /*vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);*/
}

VkCommandBuffer VulkanCommandBuffer::GetCommandBuffer() const
{
    return commandBuffer;
}

void VulkanCommandBuffer::RecordCommandBuffer(uint32_t imageIndex,Renderer& renderer)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    // Assume DrawFrame function is a member function of your VulkanBase class
    // You may need to modify this part based on your actual implementation
    renderer.DrawFrame(imageIndex, commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void VulkanCommandBuffer::CreateCommandBuffer(const CommandPool& commandPool,const VkDevice device)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}
