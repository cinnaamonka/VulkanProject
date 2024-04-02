#include "Pipeline2D.h"
#include "../GraphicsPipeline.h"
#include "../DescriptorSetManager.h"


Pipeline::Pipeline() :
	m_Shader{ "shaders/shader.vert.spv", "shaders/shader.frag.spv" }
{

}

void Pipeline::Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkFormat& swapChainImageFormat,
	std::vector<VkImageView>& swapChainImageViews, const VkExtent2D& swapChainExtent,
	const QueueFamilyIndices& queueFamilyIndexes, const VkQueue& graphicsQueue)
{
	m_RenderPass.CreateRenderPass(device, swapChainImageFormat);

	m_Shader.Init(device, physicalDevice, m_RenderPass, swapChainExtent);

	m_GraphicsPipeline.CreateGraphicsPipeline(device, physicalDevice, m_Shader, m_RenderPass, m_VulkanContext,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		DAEDataBuffer::GetDeviceSize(), m_Shader.GetDescriptorSetLayout(), swapChainExtent);

	m_GraphicsPipeline.CreateFrameBuffers(device, swapChainImageViews, swapChainExtent, m_RenderPass);

	m_CommandPool.CreateCommandPool(device, queueFamilyIndexes);

	RectMesh m_RectMesh{ {-0.25,-0.25},0.5,0.3 };
	Oval m_OvalMesh{ { -0.25f,0.8f},0.2f,6 };
	RoundedRect m_RoundedRectMesh{ {0.25,0.25},0.3,0.2,0.1,21 };

	m_Scene.AddMesh(m_RectMesh, physicalDevice, device, graphicsQueue, m_CommandPool);
	m_Scene.AddMesh(m_OvalMesh, physicalDevice, device, graphicsQueue, m_CommandPool);
	m_Scene.AddMesh(m_RoundedRectMesh, physicalDevice, device, graphicsQueue, m_CommandPool);

	m_CommandBuffer = m_CommandPool.CreateCommandBuffer(device);
}

void Pipeline::DestroyPipeline(const VkDevice& device)
{
	m_CommandPool.DestroyCommandPool(device);
	m_GraphicsPipeline.DestroySwapChainFramebuffers(device);
	m_GraphicsPipeline.DestroyDescriptorSetLayout(device, m_Shader.GetDescriptorSetLayout());
	m_GraphicsPipeline.DestroyGraphicsPipeline(device);
	m_GraphicsPipeline.DestroyPipelineLayout(device);
	m_RenderPass.DestroyRenderPass(device);
}

void Pipeline::DestroyMeshes(const VkDevice device)
{
	m_Scene.DestroyMeshes(device);
	DestroyUniformBuffers(device);
}

void Pipeline::DestroyUniformBuffers(const VkDevice device)
{
	//m_Scene.DestroyUniformBuffer(device);
}

void Pipeline::Record(const VkExtent2D& swapChainExtent, uint32_t imageIndex)
{
	m_CommandBuffer.ResetCommandBuffer();

	m_CommandBuffer.RecordCommandBuffer();

	DrawScene(swapChainExtent, imageIndex);

	m_CommandBuffer.EndRecordCommandBuffer();
}

void Pipeline::DrawScene(const VkExtent2D& swapChainExtent, uint32_t imageIndex)
{
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_RenderPass.GetRenderPass();
	renderPassInfo.framebuffer = m_GraphicsPipeline.GetSwapChainBuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(m_CommandBuffer.GetVkCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(m_CommandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline.GetGraphicsPipeline());

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(m_CommandBuffer.GetVkCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(m_CommandBuffer.GetVkCommandBuffer(), 0, 1, &scissor);

	m_GraphicsPipeline.BindPoolDescriptorSet(m_CommandBuffer.GetVkCommandBuffer());

	m_Scene.DrawMesh(m_GraphicsPipeline.GetPipelineLayout(), m_CommandBuffer.GetVkCommandBuffer());
	vkCmdEndRenderPass(m_CommandBuffer.GetVkCommandBuffer());
}
