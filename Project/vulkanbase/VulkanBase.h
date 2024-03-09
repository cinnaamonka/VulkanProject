#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "VulkanUtil.h"


#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <set>
#include <limits>
#include <algorithm>


#include "GP2Shader.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "Mesh.h"
#include "Rect.h"
#include "Oval.h"
#include "RoundedRect.h"
#include "Scene.h"
#include "GraphicsPipeline.h"
#include "RenderPass.h"

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanBase
{
public:
	VulkanBase();

	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		Cleanup();
	}

private:
	void initVulkan()
	{
		// week 06
		createInstance();
		setupDebugMessenger();
		createSurface();

		// week 05
		pickPhysicalDevice();
		createLogicalDevice();

		// week 04 
		createSwapChain();
		createImageViews();

		// week 03
		m_GradientShader.Init(device);
		m_RenderPass.CreateRenderPass(device,swapChainImageFormat);
		m_GraphicsPipeline.CreateGraphicsPipeline(device, m_GradientShader, m_RenderPass);
		m_GraphicsPipeline.CreateFrameBuffers(device, swapChainImageViews, swapChainExtent, m_RenderPass);

		// week 02  
		m_CommandPool.CreateCommandPool(device, FindQueueFamilies(physicalDevice));

		m_Scene.AddMesh(m_RectMesh, physicalDevice, device);
		m_Scene.AddMesh(m_OvalMesh, physicalDevice, device);
		m_Scene.AddMesh(m_RoundedRectMesh, physicalDevice, device);
		m_CommandBuffer = m_CommandPool.CreateCommandBuffer(device);

		// week 06
		createSyncObjects();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			// week 06
			DrawFrame();
		}
		vkDeviceWaitIdle(device);
	}

	void Cleanup()
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

	void createSurface()
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}

	// Week 01: 
	// Actual window
	// simple fragment + vertex shader creation functions
	// These 5 functions should be refactored into a separate C++ class
	// with the correct internal state.

	GLFWwindow* window;
	void initWindow();

	GP2Shader m_GradientShader{ "shaders/shader.vert.spv", "shaders/shader.frag.spv" };

	// Week 02
	// Queue families
	// CommandBuffer concept

	CommandPool m_CommandPool;
	VulkanCommandBuffer m_CommandBuffer;

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	void DrawFrame(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	Mesh m_Mesh{};
	Scene m_Scene{};

	// Week 03
	// Renderpass concept
	// Graphics pipeline

	RectMesh m_RectMesh{ {-0.25,-0.25},0.5,0.3 };
	Oval m_OvalMesh{ { -0.25f,0.25f},0.2f,20 };
	RoundedRect m_RoundedRectMesh{ {0.25,0.25},0.3,0.2,0.1,20 };

	//VkPipelineLayout pipelineLayout;
	/*VkPipeline graphicsPipeline;*/
	/*VkRenderPass renderPass;*/

	GraphicsPipeline m_GraphicsPipeline;
	RenderPass m_RenderPass;

	//void createFrameBuffers();
	//void createRenderPass();
	//void createGraphicsPipeline();

	// Week 04
	// Swap chain and image view support

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	void createSwapChain();
	void createImageViews();

	// Week 05 
	// Logical and physical device

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();

	// Week 06
	// Main initialization

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createInstance();

	void createSyncObjects();
	void DrawFrame();


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};