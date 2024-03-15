#include "VulkanBase.h"
#include "../Engine/Rect.h"

#include <functional>

VulkanBase::VulkanBase()
{
}

void VulkanBase::Run()
{
	initWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void VulkanBase::InitVulkan()
{
	// week 06
	createInstance();
	setupDebugMessenger();
	CreateSurface();

	// week 05
	pickPhysicalDevice();
	createLogicalDevice();

	// week 04 
	createSwapChain();
	createImageViews();

	// week 03
	m_GradientShader.Init(device);
	m_RenderPass.CreateRenderPass(device, swapChainImageFormat);
	m_GraphicsPipeline.CreateGraphicsPipeline(device, m_GradientShader, m_RenderPass);
	m_GraphicsPipeline.CreateFrameBuffers(device, swapChainImageViews, swapChainExtent, m_RenderPass);

	// week 02  
	m_CommandPool.CreateCommandPool(device, FindQueueFamilies(physicalDevice));

	m_Scene.AddMesh(m_RectMesh, physicalDevice, device,graphicsQueue,m_CommandPool);
	m_Scene.AddMesh(m_OvalMesh, physicalDevice, device, graphicsQueue, m_CommandPool);
	m_Scene.AddMesh(m_RoundedRectMesh, physicalDevice, device, graphicsQueue, m_CommandPool);
	m_CommandBuffer = m_CommandPool.CreateCommandBuffer(device);

	// week 06
	createSyncObjects();
}

void VulkanBase::MainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// week 06
		DrawFrame();
	}
	vkDeviceWaitIdle(device);
}

void VulkanBase::Cleanup()
{
	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroyFence(device, inFlightFence, nullptr);

	m_CommandPool.DestroyCommandPool(device);

	m_GraphicsPipeline.DestroySwapChainFramebuffers(device);
	m_GraphicsPipeline.DestroyGraphicsPipeline(device);
	m_GraphicsPipeline.DestroyPipelineLayout(device);
	m_RenderPass.DestroyRenderPass(device);

	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	m_Scene.DestroyMeshes(device);
	vkDestroyDevice(device, nullptr);

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();


}

void VulkanBase::CreateSurface()
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}


void VulkanBase::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}
void VulkanBase::DrawFrame(VkCommandBuffer commandBuffer, uint32_t imageIndex)
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

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline.GetGraphicsPipeline());

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

	m_Scene.DrawMesh(commandBuffer);
	vkCmdEndRenderPass(commandBuffer);
}


QueueFamilyIndices VulkanBase::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;

	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}