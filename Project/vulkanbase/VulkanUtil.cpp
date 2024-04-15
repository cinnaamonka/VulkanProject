#include "VulkanUtil.h"
#include "../Engine/Structs.h"
#include <set>
#include <string>
#include <unordered_map>
#include <functional>


#define TINYOBJLOADER_IMPLEMENTATION
#include <../tinyobjloader-src/tiny_obj_loader.h>

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device,const VkSurfaceKHR& surface)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;

	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.IsComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}
bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void LoadModel(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices,Mesh3D& mesh)
{
	// Clear the vectors in case they aren't empty
	vertices.clear();
	indices.clear();

	// Create needed objects to read in .obj file
	tinyobj::attrib_t attrib{};
	std::vector<tinyobj::shape_t> shapes{};
	std::vector<tinyobj::material_t> materials{};

	// Create objects for error throwing
	std::string warn, err;

	// Read file, returned false, throw error
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex3D, uint32_t> uniqueVertices{};

	// Loop trough every shape that was read from the file
	for (const auto& shape : shapes)
	{
		// Loop trough all indices in current shape
		for (const auto& index : shape.mesh.indices)
		{
			// Create empty vertex
			Vertex3D vertex{};

			// Add position to vertex
			vertex.position = {
				attrib.vertices[static_cast<uint64_t>(3) * index.vertex_index],
				attrib.vertices[static_cast<uint64_t>(3) * index.vertex_index + static_cast<uint64_t>(1)],
				attrib.vertices[static_cast<uint64_t>(3) * index.vertex_index + static_cast<uint64_t>(2)]
			};

			// Add normal to vertex
			vertex.normal = {
				attrib.normals[static_cast<uint64_t>(3) * index.normal_index],
				attrib.normals[static_cast<uint64_t>(3) * index.normal_index + static_cast<uint64_t>(1)],
				attrib.normals[static_cast<uint64_t>(3) * index.normal_index + static_cast<uint64_t>(2)]
			};

			// Add color to vertex
			vertex.color = {
				attrib.colors[static_cast<uint64_t>(3) * index.vertex_index],
				attrib.colors[static_cast<uint64_t>(3) * index.vertex_index + static_cast<uint64_t>(1)],
				attrib.colors[static_cast<uint64_t>(3) * index.vertex_index + static_cast<uint64_t>(2)]
			};

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}

	mesh.SetVertices(vertices);
	mesh.SetModelIndexes(indices);
}

