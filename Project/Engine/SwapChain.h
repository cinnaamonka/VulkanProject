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
    SwapChain();
       
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
        return m_SwapChainImageFormat;
    }

    std::vector<VkImageView>& GetSwapChainImageViews()
    {
        return m_SwapChainImageViews;
    }

    const VkExtent2D& GetSwapChainExtent()
    {
        return m_SwapChainExtent;
    }
    const VkSwapchainKHR& GetSwapChain()
    {
        return m_SwapChain;
    }

private:
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkSwapchainKHR m_SwapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat m_SwapChainImageFormat;
    VkExtent2D m_SwapChainExtent;

    std::vector<VkImageView> m_SwapChainImageViews;


};