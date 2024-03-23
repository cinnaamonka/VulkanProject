#include "GPUObject.h"
#include <vector>
#include <stdexcept>
#include "Utils/Structs.h"
#include "vulkanbase/VulkanUtil.h"
#include <set>

void GPUObject::PickPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices{ deviceCount };
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	for (const auto& physicalDevice : devices) 
	{
		if (IsDeviceSuitable(physicalDevice, surface))
		{
			m_PhysicalDevice = physicalDevice;
			break;
		}
	}

	if (m_PhysicalDevice == VK_NULL_HANDLE) 
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool GPUObject::IsDeviceSuitable(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface);
	bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);
	return indices.isComplete() && extensionsSupported;
}

void GPUObject::CreateLogicalDevice(VkDevice& device, const VkSurfaceKHR& surface)
{
	QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice,surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &m_PresentQueue);
}
