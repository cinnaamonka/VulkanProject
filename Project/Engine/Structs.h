#pragma once
#include <vulkan/vulkan_core.h>
#include  <optional>
#include  <iostream>
#include <glm/glm.hpp>
#include <array>
#include <functional>
#include <vector>

const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};
struct QueueFamilyIndices 
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};
struct VertexUBO
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};
struct Vertex3D
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color{ 1.f,1.f,1.f };
	glm::vec2 texCoord;

	bool operator==(const Vertex3D& other) const 
	{
		return position == other.position && color == other.color && normal == other.normal;
	}

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex3D);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

		// Attribute 0: Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex3D, position);

		// Attribute 1: Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex3D, color);

		// Attribute 2: Normal
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT; 
		attributeDescriptions[2].offset = offsetof(Vertex3D, normal);

		//Attribute 3: texCoord
		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT; // Correct format for texture coordinates
		attributeDescriptions[3].offset = offsetof(Vertex3D, texCoord);

		return attributeDescriptions;
	}

};
namespace std {
	template<>
	struct hash<glm::vec3> {
		size_t operator()(const glm::vec3& v) const {
			size_t h1 = std::hash<float>{}(v.x);
			size_t h2 = std::hash<float>{}(v.y);
			size_t h3 = std::hash<float>{}(v.z);
			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};
}

namespace std
{
	template<> struct hash<Vertex3D>
	{
		size_t operator()(Vertex3D const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec3>()(vertex.normal) << 1);
		}
	};
}
struct ViewProjection 
{
	glm::mat4 proj;
	glm::mat4 view;
};
struct VertexConstant
{
	glm::mat4 modelMatrix;
};
struct MeshData 
{
	glm::mat4 model;
};
struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBindingDescription() 
	{
		VkVertexInputBindingDescription bindingDescription{};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		// Attribute 0
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		// Attribute 1
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}

};