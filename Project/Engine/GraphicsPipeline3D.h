#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "../Engine/3D/GP2Shader3D.h"
#include "RenderPass.h"
#include "../vulkanbase/VulkanUtil.h"
#include "DescriptorPoolManager.h"

class GraphicsPipeline3D
{
public:
	GraphicsPipeline3D() = default;
	~GraphicsPipeline3D() = default;

	GraphicsPipeline3D(const GraphicsPipeline3D& other) = delete;
	GraphicsPipeline3D& operator=(const GraphicsPipeline3D& other) = delete;
	GraphicsPipeline3D(GraphicsPipeline3D&& other) = delete;
	GraphicsPipeline3D& operator=(GraphicsPipeline3D&& other) = delete;

	void CreateGraphicsPipeline(const VkDevice& device, const VkPhysicalDevice& physicalDevice, GP2Shader3D& shader,
		const RenderPass& renderPass, const VkBufferUsageFlags& usageFlags,
		const VkMemoryPropertyFlags& memoryPropertyFlags, const VkDeviceSize& deviceSize, 
	    const VkExtent2D& swapChainExtent, ImageManager& imageManager);
	void CreateFrameBuffers(const VkDevice& device, std::vector<VkImageView>& swapChainImageViews,
		const VkExtent2D& swapChainExtent, const RenderPass& renderPass); 
	 
	void DestroySwapChainFramebuffers(const VkDevice& device);
	void DestroyGraphicsPipeline(const VkDevice& device);
	void DestroyPipelineLayout(const VkDevice& device);
	void DestroyDescriptorSetLayout(const VkDevice& device);

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
