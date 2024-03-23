#pragma once
#include "vulkan/vulkan.h"
#include "../vulkanbase/VulkanUtil.h"


class GPUObject
{
public:
	GPUObject() = default;
	~GPUObject() = default;

	GPUObject(const GPUObject& other) = delete;
	GPUObject& operator=(const GPUObject& other) = delete;
	GPUObject(GPUObject&& other) = delete;
	GPUObject& operator=(GPUObject&& other) = delete;

	void PickPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface);
	bool IsDeviceSuitable(const VkPhysicalDevice& physicalDevice,const VkSurfaceKHR& surface);
	void CreateLogicalDevice(VkDevice& device, const VkSurfaceKHR& surface);

	const VkQueue& GetGraphicsQueue()
	{
		return m_GraphicsQueue;
	}

	const VkQueue& GetPresentQueue()
	{
		return m_PresentQueue;
	}

	const VkPhysicalDevice& GetPhysicalDevice()
	{
		return m_PhysicalDevice;
	}

private:

	VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	VkQueue m_GraphicsQueue;
	VkQueue m_PresentQueue;

};