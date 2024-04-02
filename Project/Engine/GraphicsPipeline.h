#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "../Engine/2D/GP2Shader2D.h"
#include "RenderPass.h"
#include "DescriptorPoolManager.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline() = default;
	~GraphicsPipeline() = default;

	GraphicsPipeline(const GraphicsPipeline& other) = delete;
	GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;
	GraphicsPipeline(GraphicsPipeline&& other) = delete;
	GraphicsPipeline& operator=(GraphicsPipeline&& other) = delete;

	void CreateGraphicsPipeline(const VkDevice& device, const VkPhysicalDevice& physicalDevice, GP2Shader& shader,
		const RenderPass& renderPass, const VulkanContext& context, const VkBufferUsageFlags& usageFlags,
		const VkMemoryPropertyFlags& memoryPropertyFlags, const VkDeviceSize& deviceSize,
		VkDescriptorSetLayout& descriptorSetLayout, const VkExtent2D& swapChainExtent);
	void CreateFrameBuffers(const VkDevice& device, std::vector<VkImageView>& swapChainImageViews,
		const VkExtent2D& swapChainExtent, const RenderPass& renderPass);

	void DestroySwapChainFramebuffers(const VkDevice& device);
	void DestroyGraphicsPipeline(const VkDevice& device);
	void DestroyPipelineLayout(const VkDevice& device);
	void DestroyDescriptorSetLayout(const VkDevice& device, VkDescriptorSetLayout& descriptorSetLayout);

	const  std::vector<VkFramebuffer>& GetSwapChainBuffers()
	{
		return m_SwapChainFramebuffers;
	}

	const VkPipeline& GetGraphicsPipeline()
	{
		return m_GraphicsPipeline;
	}

	void SetUBO(ViewProjection ubo, size_t uboIndex);

	VkPushConstantRange CreatePushConstantRange();

	void BindPoolDescriptorSet(const VkCommandBuffer& commandBuffer);

	const VkPipelineLayout& GetPipelineLayout()
	{
		return m_PipelineLayout;
	}

private:
	VkPipelineLayout m_PipelineLayout;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	VkPipeline m_GraphicsPipeline{};
	std::unique_ptr<DAEDescriptorPool<ViewProjection>> m_UBOPool;
};
