#include "vulkanbase/VulkanUtil.h"
#include "CommandPool.h"
#include "Structs.h"

CommandPool::CommandPool()
{
}

CommandPool::~CommandPool()
{

}

QueueFamilyIndices CommandPool::FindQueueFamilies(const VkPhysicalDevice& device,const VkSurfaceKHR& surface)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) 
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

void CommandPool::CreateCommandPool(const VkDevice& device, const VkPhysicalDevice& physicalDevice,const VkSurfaceKHR& surface)
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice, surface);

	VkCommandPoolCreateInfo poolInfo{};

	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	//Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
	//Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
	//We will be recording a command buffer every frame, so we want to be able to reset and rerecord over it
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void CommandPool::DestroyCommandPool(const VkDevice& device)
{
	if (m_CommandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device, m_CommandPool, nullptr);
		m_CommandPool = VK_NULL_HANDLE;
	}
}

VkCommandPool CommandPool::GetCommandPool() const
{
	if (m_CommandPool)
	{
		return m_CommandPool;
	}
	
}