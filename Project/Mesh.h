#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Structs.h"

class Mesh
{
public:
	void DestroyMesh(const VkDevice& device);

	void Draw(const VkCommandBuffer& buffer,const VkPipeline pipeline);

	const VkBuffer& GetBuffer() const
	{
		return m_VkBuffer;
	}
	const VkDeviceMemory& GetMemory() const
	{
		return m_VkDeviceMemory;
	}
	const std::vector<Vertex>& GetVertices() const
	{
		return m_Vertices;
	}
	 
private:
	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice,
							uint32_t typeFilter,
							VkMemoryPropertyFlags properties);
protected:
	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const std::vector<Vertex> vertices);

	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkDeviceMemory;
	std::vector<Vertex>m_Vertices;
};