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

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, const VkSurfaceKHR& surface)
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
Vertex3D loadVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index)
{
	Vertex3D vertex{};
	vertex.position = {
		attrib.vertices[3 * index.vertex_index + 0],
		attrib.vertices[3 * index.vertex_index + 1],
		attrib.vertices[3 * index.vertex_index + 2]
	};
	vertex.normal = {
		attrib.normals[3 * index.normal_index + 0],
		attrib.normals[3 * index.normal_index + 1],
		attrib.normals[3 * index.normal_index + 2]
	};
	vertex.color = {
		attrib.colors[3 * index.vertex_index + 0],
		attrib.colors[3 * index.vertex_index + 1],
		attrib.colors[3 * index.vertex_index + 2]
	};
	vertex.texCoord = {
		attrib.texcoords[2 * index.texcoord_index + 0],
		attrib.texcoords[2 * index.texcoord_index + 1]
	};
	return vertex;
}

void LoadModel(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, Mesh3D& mesh)
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

	// Temporary storage for tangent calculation
	std::vector<glm::vec3> tangents(vertices.size(), glm::vec3(0.0f));

	// Loop through every shape that was read from the file
	for (const auto& shape : shapes) {
		// Loop through all indices in the current shape
		for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
			tinyobj::index_t idx0 = shape.mesh.indices[i];
			tinyobj::index_t idx1 = shape.mesh.indices[i + 1];
			tinyobj::index_t idx2 = shape.mesh.indices[i + 2];

			Vertex3D v0 = loadVertex(attrib, idx0);
			Vertex3D v1 = loadVertex(attrib, idx1);
			Vertex3D v2 = loadVertex(attrib, idx2);

			glm::vec3 edge1 = v1.position - v0.position;
			glm::vec3 edge2 = v2.position - v0.position;

			glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
			glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			glm::vec3 tangent;
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			v0.tangent += tangent;
			v1.tangent += tangent;
			v2.tangent += tangent;

			if (uniqueVertices.count(v0) == 0) {
				uniqueVertices[v0] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(v0);
			}
			indices.push_back(uniqueVertices[v0]);

			if (uniqueVertices.count(v1) == 0) {
				uniqueVertices[v1] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(v1);
			}
			indices.push_back(uniqueVertices[v1]);

			if (uniqueVertices.count(v2) == 0) {
				uniqueVertices[v2] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(v2);
			}
			indices.push_back(uniqueVertices[v2]);
		}
	}

	// Normalize tangents
	for (auto& vertex : vertices) {
		vertex.tangent = glm::normalize(vertex.tangent);
	}

	mesh.SetVertices(vertices);
	mesh.SetModelIndexes(indices);
}

