#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Structs.h"

class Mesh
{
public:
	Mesh() {};
	virtual ~Mesh() {};

	void DestroyMesh(const VkDevice& device);

	void Draw(const VkCommandBuffer& buffer) const;

	void AddVertex(glm::vec2 pos, glm::vec3 color);

	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const std::vector<Vertex> vertexes);
	 
private:
	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice,
							uint32_t typeFilter,
							VkMemoryPropertyFlags properties);
	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkDeviceMemory;

protected:
	std::vector<Vertex>m_Vertices;
};