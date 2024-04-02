#include "vulkanbase/VulkanBase.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Engine/GraphicsPipeline3D.h"

void VulkanBase::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


void VulkanBase::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void VulkanBase::createSyncObjects() {
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}

}

void VulkanBase::DrawFrame()
{
	//Synchronization - fences
	vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &inFlightFence);

	// get next image to draw
	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, m_SwapChain.GetSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	ViewProjection vp{};
	glm::vec3 scaleFactors(1 / 400.0f, 1 / 300.0f, 1.0f);

	glm::vec3 cameraPos = glm::vec3(10 * cosf(1), -6, 10 * sinf(1));
	glm::vec3 targetPos = glm::vec3(0, 0, 0);
	glm::vec3 upVector = glm::vec3(0, 1, 0);

	float windowWidth = 800.0f;
	float windowHeight = 600.0f;
	float aspectRatio = windowWidth / windowHeight;

	// Field of View
	float fov = 45.0f; // In degrees

	// Near and Far planes
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	// View matrix
	vp.view = glm::lookAt(cameraPos, targetPos, upVector);
	vp.proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

	m_DAEPipeline3D.GetGraphicsPipeline().SetUBO(vp, 0);
	m_DAEPipeline.GetGraphicsPipeline().SetUBO(vp, 0);
	m_DAEPipeline.Record(m_SwapChain.GetSwapChainExtent(), imageIndex);
	m_DAEPipeline3D.Record(m_SwapChain.GetSwapChainExtent(), imageIndex);

	VkSubmitInfo submitInfo1{};
	submitInfo1.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores1[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages1[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo1.waitSemaphoreCount = 1;
	submitInfo1.pWaitSemaphores = waitSemaphores1;
	submitInfo1.pWaitDstStageMask = waitStages1;

	m_DAEPipeline.GetCommandBuffer().SubmitCommandBuffer(submitInfo1);

	// Submit command buffer for the second pipeline
	VkSubmitInfo submitInfo2{};
	submitInfo2.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores2[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages2[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo2.waitSemaphoreCount = 1;
	submitInfo2.pWaitSemaphores = waitSemaphores2;
	submitInfo2.pWaitDstStageMask = waitStages2;

	m_DAEPipeline3D.GetCommandBuffer().SubmitCommandBuffer(submitInfo2); 

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo1.signalSemaphoreCount = 1;
	submitInfo1.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(m_DeviceManager.GetGraphicsQueue(), 1, &submitInfo1, inFlightFence) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkSemaphore signalSemaphores2[] = { renderFinishedSemaphore };
	submitInfo2.signalSemaphoreCount = 1;
	submitInfo2.pSignalSemaphores = signalSemaphores2;

	if (vkQueueSubmit(m_DeviceManager.GetGraphicsQueue(), 1, &submitInfo2, inFlightFence) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	//wait and present
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores2;

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

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}
}