#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include "Structs.h"

class VulkanCommandBuffer;

class CommandPool
{
public:
	CommandPool() :
		m_CommandPool{ VK_NULL_HANDLE }
	{}

	~CommandPool() = default;

	CommandPool(const CommandPool& other) = delete;
	CommandPool(CommandPool&& other) noexcept = delete;
	CommandPool& operator=(const CommandPool& other) = delete;
	CommandPool& operator=(CommandPool&& other) noexcept = delete;

	void CreateCommandPool(const VkDevice& device, const QueueFamilyIndices& familyIndices);
	
	const VkCommandPool& GetCommandPool() const
	{
		return m_CommandPool;
	}

	void DestroyCommandPool(const VkDevice& device);

	VulkanCommandBuffer CreateCommandBuffer(const VkDevice& device) const;  

private:

    VkCommandPool m_CommandPool;
    
};
