#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include "Structs.h"

class CommandPool
{
public:
	CommandPool();
	~CommandPool();

	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device,const VkSurfaceKHR& surface);
	
	VkCommandPool GetCommandPool() const;

	void CreateCommandPool(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

	void DestroyCommandPool(const VkDevice& device);
private:

    VkCommandPool m_CommandPool;
	
    
};
