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


#include "../Engine/GP2Shader.h"
#include "../Engine/CommandPool.h"
#include "../Engine/CommandBuffer.h"
#include "../Engine/Mesh.h"
#include "../Engine/Rect.h"
#include "../Engine/Oval.h"
#include "../Engine/RoundedRect.h"
#include "../Engine/Scene.h"
#include "../Engine/GraphicsPipeline.h"
#include "../Engine/RenderPass.h"
#include "../Engine/IndexBuffer.h"
#include "../Engine/Pipeline.h"
#include "../Engine/Utils/Structs.h"
#include "../Engine/SwapChain.h"

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanBase
{
public:
	VulkanBase();

	void Run();
private:
	void InitVulkan();
	void MainLoop();
	void Cleanup();
	
	void CreateSurface();

	GLFWwindow* window;
	void initWindow();

	Pipeline m_DAEPipeline;

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	
	SwapChain m_SwapChain;

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