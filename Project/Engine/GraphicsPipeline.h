#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "GP2Shader.h"
#include "RenderPass.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline() = default;
	~GraphicsPipeline() = default;

	GraphicsPipeline(const GraphicsPipeline& other) = delete;
	GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;
	GraphicsPipeline(GraphicsPipeline&& other) = delete;
	GraphicsPipeline& operator=(GraphicsPipeline&& other) = delete;

	void CreateGraphicsPipeline(const VkDevice& device, GP2Shader& shader, const RenderPass& renderPass);
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

	static VkDescriptorSetLayout& GetDiscriptorSetLayout()
	{
		return m_DescriptorSetLayout;
	}

	void CreateDiscriptiveSetLayout(const VkDescriptorSetLayoutBinding& layoutBinding,const VkDevice& device);
private:
	VkPipelineLayout m_PipelineLayout;
	static VkDescriptorSetLayout m_DescriptorSetLayout;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	VkPipeline m_GraphicsPipeline;
};
