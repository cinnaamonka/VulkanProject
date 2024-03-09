#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Structs.h"

class Mesh
{
public:
	void DestroyMesh(const VkDevice& device);

	void Draw(const VkCommandBuffer& buffer) const;

	void AddVertex(glm::vec2 pos, glm::vec3 color);

	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device);
	 
private:
	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice,
							uint32_t typeFilter,
							VkMemoryPropertyFlags properties);

	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkDeviceMemory;
	std::vector<Vertex>m_Vertices;
};