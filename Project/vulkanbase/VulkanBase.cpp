#include "VulkanBase.h"
#include <functional>

using namespace std::placeholders;

VulkanBase::VulkanBase()
	 /*: commandBuffer(
		std::bind(static_cast<void(VulkanBase::*)(uint32_t, VkCommandBuffer)>(&VulkanBase::DrawFrame), this, _1, _2),
		device, commandPool.GetCommandPool() ) */
{

}