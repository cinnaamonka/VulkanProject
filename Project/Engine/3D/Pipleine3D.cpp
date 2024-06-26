#include "Pipeline3D.h"
#include "../GraphicsPipeline.h"
#include "../DaeDataBuffer.h"

#include <vector>


Pipeline3D::Pipeline3D() :
	m_Shader{ "shaders/3DShader.vert.spv", "shaders/3DShader.frag.spv" }
{

}

void Pipeline3D::Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkFormat& swapChainImageFormat,
	std::vector<VkImageView>& swapChainImageViews, const VkExtent2D& swapChainExtent,
	const QueueFamilyIndices& queueFamilyIndexes, const VkQueue& graphicsQueue,
	CommandPool& commandPool, Mesh3D& mesh, Mesh3D& model, Mesh3D& sphere,ImageManager& imageManager, DepthBuffer& depthBuffer)
{
	m_RenderPass.CreateRenderPass(device,physicalDevice, swapChainImageFormat, false, depthBuffer);

	m_Shader.Init(device, physicalDevice, m_RenderPass, swapChainExtent);

	m_GraphicsPipeline.CreateGraphicsPipeline(device, physicalDevice, m_Shader, m_RenderPass,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		DAEDataBuffer::GetDeviceSize(), swapChainExtent, imageManager);

	m_GraphicsPipeline.CreateFrameBuffers(device, swapChainImageViews, swapChainExtent, m_RenderPass, depthBuffer.GetDepthImageView());

	sphere.Scale({ 3.0f, 3.0f, 3.0f });
	sphere.Translate({ -20.0f, .0f, -20.0f });
	

	mesh.Initialize(physicalDevice, device, graphicsQueue, commandPool, imageManager);
	model.InitializeModel(physicalDevice, device, graphicsQueue, commandPool, imageManager);
	sphere.InitializeModel(physicalDevice, device, graphicsQueue, commandPool, imageManager);
	 
	

	m_Scene.AddMesh(mesh);
	m_Scene.AddMesh(model);
	m_Scene.AddMesh(sphere);

	m_CommandBuffer = commandPool.CreateCommandBuffer(device);
}

void Pipeline3D::DestroyPipeline(const VkDevice& device, CommandPool& commandPool)
{
	commandPool.DestroyCommandPool(device);
	m_GraphicsPipeline.DestroySwapChainFramebuffers(device);
	m_GraphicsPipeline.DestroyDescriptorSetLayout(device);
	m_GraphicsPipeline.DestroyGraphicsPipeline(device);
	m_GraphicsPipeline.DestroyPipelineLayout(device);
	m_RenderPass.DestroyRenderPass(device);
	m_Shader.DestroyDataBuffer();
}

void Pipeline3D::DestroyMeshes(const VkDevice& device)
{
	m_Scene.DestroyMeshes(device);
	DestroyUniformBuffers(device);
}
void Pipeline3D::DestroyUniformBuffers(const VkDevice& device)
{
	m_Scene.DestroyUniformBuffers(device);
}

void Pipeline3D::Record(const VkExtent2D& swapChainExtent, uint32_t imageIndex)
{
	m_CommandBuffer.ResetCommandBuffer();

	m_CommandBuffer.RecordCommandBuffer();

	DrawScene(swapChainExtent, imageIndex);

	m_CommandBuffer.EndRecordCommandBuffer();
}

void Pipeline3D::DrawScene(const VkExtent2D& swapChainExtent, uint32_t imageIndex)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_RenderPass.GetRenderPass();
	renderPassInfo.framebuffer = m_GraphicsPipeline.GetSwapChainBuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

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
