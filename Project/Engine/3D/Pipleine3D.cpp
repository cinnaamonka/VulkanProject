#include "Pipeline3D.h"
#include "../GraphicsPipeline.h"
#include "../DescriptorSetManager.h"
#include "../DaeDataBuffer.h"


Pipeline3D::Pipeline3D() :
	m_Shader{ "shaders/3DShader.vert.spv", "shaders/3DShader.frag.spv" }
{

}

void Pipeline3D::Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkFormat& swapChainImageFormat,
	std::vector<VkImageView>& swapChainImageViews, const VkExtent2D& swapChainExtent,
	const QueueFamilyIndices& queueFamilyIndexes, const VkQueue& graphicsQueue) 
{
	m_RenderPass.CreateRenderPass(device, swapChainImageFormat);
	m_Shader.Init(device, physicalDevice,m_RenderPass,swapChainExtent);
	m_GraphicsPipeline.CreateGraphicsPipeline(device, physicalDevice, m_Shader, m_RenderPass, m_VulkanContext, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,DAEDataBuffer::GetDeviceSize(),
		m_Shader.GetDescriptorSetLayout(),swapChainExtent); 
	m_GraphicsPipeline.CreateFrameBuffers(device, swapChainImageViews, swapChainExtent, m_RenderPass);
	m_CommandPool.CreateCommandPool(device, queueFamilyIndexes);

	Mesh3D cubeMesh;
	
	glm::vec3 vertices[] = {
		glm::vec3(-0.2f, -0.2f, -0.2f),
		glm::vec3(-0.2f, -0.2f, 0.2f), 
		glm::vec3(-0.2f, 0.2f, -0.2f), 
		glm::vec3(-0.2f, 0.2f, 0.2f),  
		glm::vec3(0.2f, -0.2f, -0.2f), 
		glm::vec3(0.2f, -0.2f, 0.5f),  
		glm::vec3(0.2f, 0.2f, -0.2f),  
		glm::vec3(0.2f, 0.2f, 0.2f)    
	};

	for (int i = 0; i < 8; ++i) {
		cubeMesh.AddVertex(vertices[i], glm::vec3(1.0f)); // Assuming color is white for all vertices
	}

	cubeMesh.AddTriangle(0, 1, 2); 
	cubeMesh.AddTriangle(1, 3, 2);

	cubeMesh.AddTriangle(4, 0, 6); 
	cubeMesh.AddTriangle(0, 2, 6);

	cubeMesh.AddTriangle(5, 4, 7); 
	cubeMesh.AddTriangle(4, 6, 7);

	cubeMesh.AddTriangle(1, 5, 3); 
	cubeMesh.AddTriangle(5, 7, 3);

	cubeMesh.AddTriangle(4, 5, 0); 
	cubeMesh.AddTriangle(5, 1, 0);

	cubeMesh.AddTriangle(2, 3, 6); 
	cubeMesh.AddTriangle(3, 7, 6);

	cubeMesh.Initialize(physicalDevice, device, graphicsQueue, m_CommandPool);

	m_Scene.AddMesh(cubeMesh);

	m_CommandBuffer = m_CommandPool.CreateCommandBuffer(device);
}

void Pipeline3D::DestroyPipeline(const VkDevice& device)
{
	m_CommandPool.DestroyCommandPool(device);
	m_GraphicsPipeline.DestroySwapChainFramebuffers(device);
	m_GraphicsPipeline.DestroyDescriptorSetLayout(device, m_Shader.GetDescriptorSetLayout());
	m_GraphicsPipeline.DestroyGraphicsPipeline(device);
	m_GraphicsPipeline.DestroyPipelineLayout(device);
	m_RenderPass.DestroyRenderPass(device);
}  

void Pipeline3D::DestroyMeshes(const VkDevice device)
{
	m_Scene.DestroyMeshes(device);
	DestroyUniformBuffers(device);
}

void Pipeline3D::DestroyUniformBuffers(const VkDevice device)
{
	//m_Scene.DestroyUniformBuffer(device);
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

	m_Scene.DrawMesh(m_GraphicsPipeline.GetPipelineLayout(),m_CommandBuffer.GetVkCommandBuffer());
	vkCmdEndRenderPass(m_CommandBuffer.GetVkCommandBuffer());
}
