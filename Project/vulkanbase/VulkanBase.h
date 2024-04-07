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


#include "../Engine/2D/GP2Shader2D.h"
#include "../Engine/CommandPool.h"
#include "../Engine/CommandBuffer.h"
#include "../Engine/2D/Mesh2D.h"
#include "../Engine/Meshes/Rect.h"
#include "../Engine/Meshes/Oval.h"
#include "../Engine/Meshes/RoundedRect.h"
#include "../Engine/Scene.h"
#include "../Engine/GraphicsPipeline.h"
#include "../Engine/RenderPass.h"
#include "../Engine/IndexBuffer.h"
#include "../Engine/2D/Pipeline2D.h"
#include "../Engine/3D/Pipeline3D.h"
#include "../Engine/Structs.h"
#include "../Engine/SwapChain.h"
#include "../Engine/GPUObject.h"
#include "../Engine/DeviceManager.h"
#include "../Engine/Camera.h"

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

	GLFWwindow* m_Window;
	void initWindow();
	void OnKeyEvent(int key, int scancode, int action, int mods);
	void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
	void OnMouseEvent(GLFWwindow* window, int button, int action, int mods);

	Camera m_Camera;
	float m_CameraRadius;
	glm::vec3 m_DragStart;
	float m_Rotation;
	double m_Lag = 0;

	Pipeline m_DAEPipeline;
	Pipeline3D m_DAEPipeline3D;
	
	SwapChain m_SwapChain;
	DeviceManager m_DeviceManager;

	CommandPool m_CommandPool;

	Mesh3D m_CubeMesh;
	Mesh3D m_Model;
	const std::string MODEL_PATH = "models/viking_room.obj";

	// Week 06
	// Main initialization

	VkInstance m_Instance;
	VkDebugUtilsMessengerEXT m_DebugMessenger;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR m_Surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkSemaphore renderFinishedSemaphore2;
	VkFence m_InFlightFence;
	VkFence m_InFlightFence2;

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();

	void createInstance();

	void createSyncObjects();
	void DrawFrame();


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};