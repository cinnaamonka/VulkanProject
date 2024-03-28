//#pragma once
//
//#include "vulkanbase/VulkanUtil.h"
//#include "DAEDataBuffer.h"
//
//template <class UBO>
//class UniformBufferObject
//{
//public:
//	void Initialize(const VulkanContext& context);
//	void Upload();
//	void SetData(const UBO& ubo) 
//	{
//		m_UBOSrc = ubo;
//	}
//
//	VkBuffer& GetVkBuffer() 
//	{
//		return m_UBOBuffer->getVkBuffer();
//	}
//private:
//	std::unique_ptr<DAEDataBuffer> m_UBOBuffer;
//	UBO m_UBOSrc;
//};
//
//template<class UBO>
//inline void UniformBufferObject<UBO>::Initialize(const VulkanContext& context)
//{
//	m_UBOBuffer = std::make_unique<DAEDataBuffer>(
//		context,
//		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//		sizeof(UBO)
//	);
//}
//
//template<class UBO>
//inline void UniformBufferObject<UBO>::Upload()
//{
//	m_UBOBuffer->Upload(sizeof(UBO), &m_UBOSrc);
//}
//
//template<class UBO>
//using DAEUniformBufferObjectPtr = std::unique_ptr<DAEUniformBufferObject<UBO>>;
//
//
