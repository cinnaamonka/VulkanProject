#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <functional>

class CommandPool;
class Renderer;

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer();

	~VulkanCommandBuffer();

	void CreateCommandBuffer(const CommandPool& commandPool, const VkDevice device);
	VkCommandBuffer GetCommandBuffer() const;

	void RecordCommandBuffer(uint32_t imageIndex, Renderer& renderer);


private:

	VkCommandBuffer commandBuffer;

};
