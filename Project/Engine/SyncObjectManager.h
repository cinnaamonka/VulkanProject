#pragma once

#include <vulkan/vulkan.h>
#include <vector>


class SyncObjectManager final
{
public:
	SyncObjectManager() = delete;
	SyncObjectManager(VkDevice device, uint32_t maxFrames);
	~SyncObjectManager() = default;
	SyncObjectManager(SyncObjectManager& other) = delete;
	SyncObjectManager(SyncObjectManager&& other) = delete;
	SyncObjectManager& operator=(SyncObjectManager& other) = delete;
	SyncObjectManager& operator=(SyncObjectManager&& other) = delete;

	void Cleanup(VkDevice device);

	VkSemaphore& GetImageAvailableSemaphore(uint32_t frame) { return m_ImageAvailableSemaphores[frame]; }
	VkSemaphore& GetRenderFinishedSemaphore(uint32_t frame) { return m_RenderFinishedSemaphores[frame]; }
	VkFence& GetInFlightFence(uint32_t frame) { return m_InFlightFences[frame]; }

private:

	std::vector<VkSemaphore> m_ImageAvailableSemaphores{};

	std::vector<VkSemaphore> m_RenderFinishedSemaphores{};
	std::vector<VkFence> m_InFlightFences{};
	void CreateSyncObjects(VkDevice device, uint32_t maxFrames);
};
