#pragma once
#include "vulkanbase/VulkanUtil.h"

class VulkanCommandBuffer;

class Renderer
{
public:
	Renderer() {};
	~Renderer() {};

	void DrawScene();
	void DrawFrame(uint32_t imageIndex, const VkCommandBuffer& commandBuffer);

	void Init(const VkRenderPass& renderPass,
		      const std::vector<VkFramebuffer>& swapChain, 
		      const VkExtent2D& extend, 
		      const VkPipeline& graphicsPipeline,
		      const VkCommandBuffer& commandBuffer);
private:

	VkCommandBuffer commandBuffer;
	VkRenderPass renderPass;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkExtent2D swapChainExtent;
	VkPipeline graphicsPipeline;
};