#include "VulkanBase.h"
#include "../Engine/Meshes/Rect.h"

#include <functional>

VulkanBase::VulkanBase():
	m_Window{},
	m_Surface{},
	imageAvailableSemaphore{},
	renderFinishedSemaphore{},
	renderFinishedSemaphore2{},
	m_DebugMessenger{},
	m_InFlightFence{},
	m_InFlightFence2{},
	m_Instance{},
	m_DeviceManager{}
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

	m_DeviceManager.PickPhysicalDevice(m_Instance, m_Surface);
	m_DeviceManager.CreateLogicalDevice(device, m_Surface);

	m_SwapChain.CreateSwapChain(m_Surface, m_Window, FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface),device, m_DeviceManager.GetPhysicalDevice());
	m_SwapChain.CreateImageViews(device); 

	m_CommandPool.CreateCommandPool(device, FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface));

	m_DAEPipeline.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(),
		m_SwapChain.GetSwapChainExtent(), FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface),
		m_DeviceManager.GetGraphicsQueue(),m_CommandPool);

	m_DAEPipeline3D.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(),m_SwapChain.GetSwapChainExtent(),
		FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface), m_DeviceManager.GetGraphicsQueue(), m_CommandPool);

	// week 06
	createSyncObjects();
}

void VulkanBase::MainLoop()
{
	while (!glfwWindowShouldClose(m_Window))
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
	vkDestroySemaphore(device, renderFinishedSemaphore2, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroyFence(device, m_InFlightFence, nullptr);
	vkDestroyFence(device, m_InFlightFence2, nullptr);

	m_DAEPipeline.DestroyPipeline(device, m_CommandPool);
	m_DAEPipeline3D.DestroyPipeline(device, m_CommandPool);

	for (auto imageView : m_SwapChain.GetSwapChainImageViews())
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	}
	vkDestroySwapchainKHR(device, m_SwapChain.GetSwapChain(), nullptr);

	m_DAEPipeline.DestroyMeshes(device);
	m_DAEPipeline3D.DestroyMeshes(device);
	
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	vkDestroyInstance(m_Instance, nullptr);

	
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void VulkanBase::CreateSurface()
{
	if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}


void VulkanBase::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}
