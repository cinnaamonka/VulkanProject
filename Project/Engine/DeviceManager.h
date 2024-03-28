#pragma once
#include <vulkan/vulkan.h>

class DeviceManager
{
public:
	DeviceManager() = default;

	~DeviceManager() = default;

	DeviceManager(const DeviceManager& other) = delete;
	DeviceManager(DeviceManager&& other) noexcept = delete;
	DeviceManager& operator=(const DeviceManager& other) = delete;
	DeviceManager& operator=(DeviceManager&& other) noexcept = delete;

	void PickPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface);
	void CreateLogicalDevice(VkDevice& device, const VkSurfaceKHR& surface);

	const VkPhysicalDevice& GetPhysicalDevice()
	{
		return m_PhysicalDevice;
	}

	const VkQueue& GetGraphicsQueue()
	{
		return m_GraphicsQueue;
	}
	const VkQueue& GetPresentQueue()
	{
		return m_PresentQueue;
	}
	 
private:
	bool IsDeviceSuitable(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
private:
	VkPhysicalDevice m_PhysicalDevice;

	VkQueue m_GraphicsQueue;
	VkQueue m_PresentQueue;
};

