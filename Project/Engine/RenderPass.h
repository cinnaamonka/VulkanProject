#pragma once
#include <vulkan/vulkan.h>

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
        return renderPass;
    }
    void CreateRenderPass(const VkDevice& device, const VkFormat& swapChainImageFormat, bool shouldClear);
    void DestroyRenderPass(const VkDevice& device);

private:
    VkRenderPass renderPass;
};