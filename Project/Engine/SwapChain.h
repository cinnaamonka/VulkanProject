#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Structs.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <limits>
#include <algorithm>

class SwapChain final
{
public:
    SwapChain() = default;
    ~SwapChain() = default;

    SwapChain(const SwapChain& other) = delete;
    SwapChain& operator=(const SwapChain& other) = delete;
    SwapChain(SwapChain&& other) = delete;
    SwapChain& operator=(SwapChain&& other) = delete;

    SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

    void CreateSwapChain(const VkSurfaceKHR& surface, GLFWwindow* window,
        const QueueFamilyIndices& queueFamilyIndexes, const VkDevice& device, const VkPhysicalDevice& physicalDevice);
    void CreateImageViews(const VkDevice& device);

    const VkFormat GetSwapChainImageFormat() const
    {
        return swapChainImageFormat;
    }

    std::vector<VkImageView>& GetSwapChainImageViews()
    {
        return swapChainImageViews;
    }

    const VkExtent2D& GetSwapChainExtent()
    {
        return swapChainExtent;
    }
    const VkSwapchainKHR& GetSwapChain()
    {
        return swapChain;
    }

private:
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;


};