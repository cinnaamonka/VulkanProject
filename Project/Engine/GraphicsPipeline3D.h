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

	void CreateGraphicsPipeline(const VkDevice& device, GP2Shader3D& shader, const RenderPass& renderPass,const VulkanContext& context);
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
	void SetUBO(ViewProjection ubo, size_t uboIndex);

	void CreateDiscriptiveSetLayout(const VkDescriptorSetLayoutBinding& layoutBinding, const VkDevice& device);
	VkPushConstantRange CreatePushConstantRange();
private:
	VkPipelineLayout m_PipelineLayout;
	static VkDescriptorSetLayout m_DescriptorSetLayout;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	VkPipeline m_GraphicsPipeline;
	std::unique_ptr<DAEDescriptorPool<ViewProjection>> m_UBOPool;
};
