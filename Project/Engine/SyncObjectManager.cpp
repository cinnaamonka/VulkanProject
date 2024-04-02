#include "SyncObjectManager.h"
#include <stdexcept>

SyncObjectManager::SyncObjectManager(VkDevice device, uint32_t maxFrames)
{
	CreateSyncObjects(device, maxFrames);
}

void SyncObjectManager::Cleanup(VkDevice device)
{
	for (size_t i = 0; i < m_ImageAvailableSemaphores.size(); ++i)
	{
		vkDestroySemaphore(device, m_ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, m_RenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(device, m_InFlightFences[i], nullptr);
	}
}

void SyncObjectManager::CreateSyncObjects(VkDevice device, uint32_t maxFrames)
{
	m_ImageAvailableSemaphores.resize(maxFrames);
	m_RenderFinishedSemaphores.resize(maxFrames);
	m_InFlightFences.resize(maxFrames);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < maxFrames; ++i)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create sync objects!");
		}
	}
}