#pragma once
#include "vulkanbase/VulkanUtil.h"
#include "DAEDataBuffer.h"
template <class UBO>
class DAEUniformBufferObject
{
public:
	void initialize(const VulkanContext& context, const VkPhysicalDevice& physicalDevice,const VkDevice& device,
		const VkBufferUsageFlags& usage,
		const VkMemoryPropertyFlags& properties,
		const VkDeviceSize& size)
	{
		// Create a buffer with appropriate size and usage flags
		m_UBOBuffer = std::make_unique<DAEDataBuffer>(physicalDevice,device,usage,properties,size); 
	};
	void upload()
	{
		m_UBOBuffer->Upload(sizeof(UBO), &m_UBOSrc);
	};
	void setData(UBO ubo)
	{
		m_UBOSrc = ubo;
	}
	VkBuffer getVkBuffer()
	{
		return m_UBOBuffer->GetVkBuffer();
	}

	void reset()
	{
		m_UBOBuffer.reset();
	}
private:
	std::unique_ptr<DAEDataBuffer> m_UBOBuffer;
	UBO m_UBOSrc;
};