#include "vulkanbase/VulkanBase.h"
#include "CommandBuffer.h"

//void VulkanBase::CreateCommandPool()
//{
//	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);
//
//	VkCommandPoolCreateInfo poolInfo{};
//
//	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//
//	//Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
//	//Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
//	//We will be recording a command buffer every frame, so we want to be able to reset and rerecord over it
//	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
//
//	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
//
//	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
//	{
//		throw std::runtime_error("failed to create command pool!");
//	}
//}


//void VulkanBase::CreateCommandBuffer()
//{
//	VkCommandBufferAllocateInfo allocInfo{};
//	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//	allocInfo.commandPool = commandPool;
//
//	//Can be submitted to a queue for execution, but cannot be called from other command buffers.
//	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//
//	// we are allocating 1 command buffer
//	allocInfo.commandBufferCount = 1;
//
//	if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS)
//	{
//		throw std::runtime_error("failed to allocate command buffers!");
//	}
//}
//
//void VulkanBase::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) 
//{
//	VkCommandBufferBeginInfo beginInfo{};
//	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//	beginInfo.flags = 0; // Optional
//	beginInfo.pInheritanceInfo = nullptr; // Optional
//
//	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
//		throw std::runtime_error("failed to begin recording command buffer!");
//	}
//	DrawFrame(imageIndex);
//
//
//	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
//		throw std::runtime_error("failed to record command buffer!");
//	}
//}

//void VulkanBase::DrawFrame(uint32_t imageIndex, VkCommandBuffer commandBuffer)
//{
//	VkRenderPassBeginInfo renderPassInfo{};
//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//	renderPassInfo.renderPass = renderPass;
//	// Using the imageIndex parameter which was passed in, we can pick the right framebuffer for the current swapchain image.
//	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
//	renderPassInfo.renderArea.offset = { 0, 0 };
//	renderPassInfo.renderArea.extent = swapChainExtent;
//
//	//The render area defines where shader loads and stores will take place. 
//	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
//	renderPassInfo.clearValueCount = 1;
//	renderPassInfo.pClearValues = &clearColor;
//
//	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//	//the second parameter specifies if the pipeline object is a graphics or compute pipeline. 
//	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
//
//	VkViewport viewport{};
//	viewport.x = 0.0f;
//	viewport.y = 0.0f;
//	viewport.width = (float)swapChainExtent.width;
//	viewport.height = (float)swapChainExtent.height;
//	viewport.minDepth = 0.0f;
//	viewport.maxDepth = 1.0f;
//	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
//
//	VkRect2D scissor{};
//	scissor.offset = { 0, 0 };
//	scissor.extent = swapChainExtent;
//	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

//	DrawScene();
//	vkCmdEndRenderPass(commandBuffer);
//}


//QueueFamilyIndices VulkanBase::FindQueueFamilies(VkPhysicalDevice device)
//{
//	QueueFamilyIndices indices;
//
//	uint32_t queueFamilyCount = 0;
//	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//
//	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//
//	int i = 0;
//	for (const auto& queueFamily : queueFamilies) {
//		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//			indices.graphicsFamily = i;
//		}
//
//		VkBool32 presentSupport = false;
//		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
//
//		if (presentSupport) {
//			indices.presentFamily = i;
//		}
//
//		if (indices.isComplete()) {
//			break;
//		}
//
//		i++;
//	}
//
//	return indices;
//}