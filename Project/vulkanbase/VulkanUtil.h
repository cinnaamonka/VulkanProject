#pragma once

#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "../Engine/Structs.h"
#include "../Engine/3D/Mesh3D.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include <vector>
#include <fstream>

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

std::vector<char> readFile(const std::string& filename);

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, const VkSurfaceKHR& surface);

bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device);

void LoadModel(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, Mesh3D& mesh); 
