#include "VulkanBase.h"
#include "../Engine/Meshes/Rect.h"
#include "../Engine/Timer.h"
#include "VulkanUtil.h"

#include <functional>
#include <chrono>
#include <thread>
#include <algorithm>


VulkanBase::VulkanBase() :
	m_Window{},
	m_Surface{},
	imageAvailableSemaphore{},
	renderFinishedSemaphore{},
	renderFinishedSemaphore2{},
	m_DebugMessenger{},
	m_InFlightFence{},
	m_InFlightFence2{},
	m_Instance{},
	m_DeviceManager{},
	m_CameraRadius{ 5 },
	m_DragStart{},
	m_Rotation{},
	m_Camera{}
{
	const float xOffset = 3.f;

	m_CubeMesh.InitializeCube(glm::vec3((-0.5f - 0.3f + xOffset) * 10.0f, -0.5f * 10.0f, -0.5f * 10.0f), 10);

	LoadModel(MODEL_PATH, m_Model.GetVertices(), m_Model.GetModelIndices(), m_Model);
}

void VulkanBase::Run()
{
	initWindow();
	InitVulkan();

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	while (doContinue)
	{
		Timer::Update();
		m_Lag += Timer::GetElapsed();

		while (m_Lag >= 1.0 / 60.0)
		{
			// fixed update here
			m_Lag -= 1.0 / 60.0;
		}
		doContinue = glfwWindowShouldClose(m_Window);
		MainLoop();
		Cleanup();

		const auto ms_per_frame = std::chrono::milliseconds(1000) / 60;

		const auto sleepTime = Timer::GetCurrent() + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();

		std::this_thread::sleep_for(sleepTime);
	}
}

void VulkanBase::InitVulkan()
{
	// week 06
	createInstance();
	setupDebugMessenger();
	CreateSurface();

	m_DeviceManager.PickPhysicalDevice(m_Instance, m_Surface);
	m_DeviceManager.CreateLogicalDevice(device, m_Surface);

	m_SwapChain.CreateSwapChain(m_Surface, m_Window, FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface), device, m_DeviceManager.GetPhysicalDevice());
	m_SwapChain.CreateImageViews(device);

	m_CommandPool.CreateCommandPool(device, FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface));

	m_DAEPipeline.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(),
		m_SwapChain.GetSwapChainExtent(), FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface),
		m_DeviceManager.GetGraphicsQueue(), m_CommandPool);


	m_DAEPipeline3D.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(), m_SwapChain.GetSwapChainExtent(),
		FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface), m_DeviceManager.GetGraphicsQueue(), m_CommandPool, m_CubeMesh, m_Model);

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

	glfwSetWindowUserPointer(m_Window, this);

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->OnKeyEvent(key, scancode, action, mods);
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->OnMouseMove(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->OnMouseEvent(window, button, action, mods);
		});
}
void VulkanBase::OnKeyEvent(int key, int scancode, int action, int mods)
{
	m_Camera.OnKeyEvent(key, scancode, action, mods, Timer::GetElapsed());
}
void VulkanBase::OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);


	if (state == GLFW_PRESS)
	{
		m_Camera.OnMouseMove(xpos, ypos, m_DragStart.x, m_DragStart.y, Timer::GetElapsed());
	}
}
void VulkanBase::OnMouseEvent(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		m_DragStart.x = static_cast<float>(xpos);
		m_DragStart.y = static_cast<float>(ypos);
	}
}
void VulkanBase::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


void VulkanBase::setupDebugMessenger()
{
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void VulkanBase::createSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore2) != VK_SUCCESS ||
		vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFence) != VK_SUCCESS ||
		vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFence2) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
}

void VulkanBase::DrawFrame()
{
	const VkFence waitFences[] = { m_InFlightFence,m_InFlightFence2 };
	vkWaitForFences(device, 1, waitFences, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, waitFences);

	// get next image to draw
	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, m_SwapChain.GetSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	float windowWidth = 800.0f;
	float windowHeight = 600.0f;
	float aspectRatio = windowWidth / windowHeight;

	// Near and Far planes
	float nearPlane = 0.001f;
	float farPlane = 100.0f;

	// 2D Camera matrix
	ViewProjection vp2D{};
	vp2D = m_Camera.GetViewProjection(windowWidth, windowHeight, nearPlane, farPlane);

	// draw pipeline 1.
	m_DAEPipeline.GetGraphicsPipeline().SetUBO(vp2D, 0);
	m_DAEPipeline.Record(m_SwapChain.GetSwapChainExtent(), imageIndex);

	ViewProjection vp3D{};

	m_Camera.Update(Timer::GetElapsed());

	vp3D = m_Camera.GetViewProjection(windowWidth, windowHeight, nearPlane, farPlane);

	m_DAEPipeline3D.GetGraphicsPipeline().SetUBO(vp3D, 0);
	m_DAEPipeline3D.Record(m_SwapChain.GetSwapChainExtent(), imageIndex);

	VkSubmitInfo submitInfo1{};
	submitInfo1.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages1[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo1.waitSemaphoreCount = 1;
	submitInfo1.pWaitSemaphores = waitSemaphores;
	submitInfo1.pWaitDstStageMask = waitStages1;

	m_DAEPipeline.GetCommandBuffer().SubmitCommandBuffer(submitInfo1);

	VkSubmitInfo submitInfo2{};
	submitInfo2.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkPipelineStageFlags waitStages2[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo2.waitSemaphoreCount = 0;
	submitInfo2.pWaitSemaphores = nullptr;
	submitInfo2.pWaitDstStageMask = waitStages2;

	m_DAEPipeline3D.GetCommandBuffer().SubmitCommandBuffer(submitInfo2);

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo1.signalSemaphoreCount = 1;
	submitInfo1.pSignalSemaphores = signalSemaphores;

	VkSemaphore signalSemaphores2[] = { renderFinishedSemaphore2 };
	submitInfo2.signalSemaphoreCount = 1;
	submitInfo2.pSignalSemaphores = signalSemaphores2;
	std::array<VkSubmitInfo, 2> submitInfos{ submitInfo1, submitInfo2 };

	if (vkQueueSubmit(m_DeviceManager.GetGraphicsQueue(), static_cast<uint32_t>(submitInfos.size()), submitInfos.data(), m_InFlightFence) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	vkQueueWaitIdle(m_DeviceManager.GetGraphicsQueue());

	VkFence fences[] = { m_InFlightFence, m_InFlightFence2 };

	vkWaitForFences(device, 2, fences, VK_TRUE, UINT64_MAX);

	std::array<VkSemaphore, 2> presentWaitSemaphores{ renderFinishedSemaphore, renderFinishedSemaphore2 };

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = static_cast<uint32_t>(presentWaitSemaphores.size());
	presentInfo.pWaitSemaphores = presentWaitSemaphores.data();

	VkSwapchainKHR swapChains[] = { m_SwapChain.GetSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(m_DeviceManager.GetPresentQueue(), &presentInfo);

}

bool checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

std::vector<const char*> VulkanBase::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}


void VulkanBase::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}
}