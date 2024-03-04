#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <functional>

#include "Structs.h"

class CommandPool;
class Renderer;

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer();

	~VulkanCommandBuffer();

	void CreateCommandBuffer(const CommandPool& commandPool, const VkDevice device);
	VkCommandBuffer GetCommandBuffer() const;

	void RecordCommandBuffer(uint32_t imageIndex, Renderer& renderer,const VkPipeline& graphicsPipeline, const VkBuffer& vertexBuffer, const std::vector<Vertex>& vertices);


private:

	VkCommandBuffer commandBuffer;

};
