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

	m_DAEPipeline.Initialize(device, physicalDevice, swapChainImageFormat, swapChainImageViews,
		swapChainExtent, FindQueueFamilies(physicalDevice), graphicsQueue);

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

	m_DAEPipeline.DestroyPipeline(device);

	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);

	m_DAEPipeline.DestroyMeshes(device);
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