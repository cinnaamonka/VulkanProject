#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <functional>

#include "Structs.h"
#include "./2D/Mesh2D.h"

class CommandPool;
class Renderer;

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer() = default;
	~VulkanCommandBuffer() = default;
	VulkanCommandBuffer(const VulkanCommandBuffer& other) = default;
	VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept = default;
	VulkanCommandBuffer& operator=(const VulkanCommandBuffer& other) = default;
	VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other) noexcept = default;


	void SubmitCommandBuffer(VkSubmitInfo& info)const;
	void RecordCommandBuffer();
	void EndRecordCommandBuffer();
	void ResetCommandBuffer() const;

	void SetVKCommandBuffer(const VkCommandBuffer& vkCommandBuffer) 
	{
		m_CommandBuffer = vkCommandBuffer;
	}

	VkCommandBuffer GetVkCommandBuffer()
	{
		return m_CommandBuffer;
	}

private:

	VkCommandBuffer m_CommandBuffer;

};
