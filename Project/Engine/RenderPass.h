#pragma once
#include <vulkan/vulkan.h>
#include <array>
#include "DepthBuffer.h"

class RenderPass
{
public:
    RenderPass() = default;
    ~RenderPass() = default;

    RenderPass(const RenderPass& other) = delete;
    RenderPass& operator=(const RenderPass& other) = delete;
    RenderPass(RenderPass&& other) = delete;
    RenderPass& operator=(RenderPass&& other) = delete;

    const VkRenderPass& GetRenderPass() const
    {
        return m_RenderPass;
    }
    void CreateRenderPass(const VkDevice& device, const VkPhysicalDevice& physicalevice, 
        const VkFormat& swapChainImageFormat, bool shouldClear, DepthBuffer& depthBuffer);
    void DestroyRenderPass(const VkDevice& device);

private:
    VkRenderPass m_RenderPass;
    std::array<VkAttachmentDescription, 2> m_Attachments;
};