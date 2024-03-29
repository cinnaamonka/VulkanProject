#include "VulkanBase.h"
#include "../Engine/Meshes/Rect.h"

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

	m_DeviceManager.PickPhysicalDevice(instance, surface);
	m_DeviceManager.CreateLogicalDevice(device,surface);

	m_SwapChain.CreateSwapChain(surface, window, FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(),surface),device, m_DeviceManager.GetPhysicalDevice());
	m_SwapChain.CreateImageViews(device); 

	m_DAEPipeline.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(),
		m_SwapChain.GetSwapChainExtent(), FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(),surface), m_DeviceManager.GetGraphicsQueue());

	m_DAEPipeline3D.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(),m_SwapChain.GetSwapChainExtent(),
		FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), surface), m_DeviceManager.GetGraphicsQueue());

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
	m_DAEPipeline3D.DestroyPipeline(device);

	for (auto imageView : m_SwapChain.GetSwapChainImageViews())
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySwapchainKHR(device, m_SwapChain.GetSwapChain(), nullptr);

	m_DAEPipeline.DestroyMeshes(device);
	m_DAEPipeline3D.DestroyMeshes(device);
	
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
