#include "Renderer.h"
#include "CommandBuffer.h"


void Renderer::DrawScene()
{
	// vertex count,instance count,first vertex,first instance
	vkCmdDraw(commandBuffer, 6, 1, 0, 0);
	
}

void Renderer::Init(const VkRenderPass& renderPass,
	const std::vector<VkFramebuffer>& swapChainFramebuffers,
	const VkExtent2D& swapChainExtent,
	const VkPipeline& graphicsPipeline,
	const VkCommandBuffer& commandBuffer)
{
	this->renderPass = renderPass;
	this->swapChainFramebuffers = swapChainFramebuffers;
	this->swapChainExtent = swapChainExtent;
	this->graphicsPipeline = graphicsPipeline;
	this->commandBuffer = commandBuffer;
}

void Renderer::DrawFrame(uint32_t imageIndex, const VkCommandBuffer& commandBuffer, const VkBuffer& vertexBuffer,const std::vector<Vertex>& vertices)
{
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = this->renderPass;
	// Using the imageIndex parameter which was passed in, we can pick the right framebuffer for the current swapchain image.
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	//The render area defines where shader loads and stores will take place. 
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	//the second parameter specifies if the pipeline object is a graphics or compute pipeline. 
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	//DrawScene();
	//mesh.Draw(commandBuffer,graphicsPipeline);

	vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

	// end of my code

	vkCmdEndRenderPass(commandBuffer);
}


