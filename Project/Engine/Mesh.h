#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "utils/Structs.h"
#include "CommandPool.h"
#include "IndexBuffer.h"

class CommandPool;

class Mesh
{
public:
	Mesh();  
	virtual ~Mesh() {};

	void DestroyMesh(const VkDevice& device);

	void Draw(const VkCommandBuffer& buffer) const;

	void AddVertex(glm::vec2 pos, glm::vec3 color);

	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const std::vector<Vertex> vertexes,
		const VkQueue& graphicsQueue,const CommandPool& commandPool, std::vector<uint16_t> indices);
	 
private:
	
	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkDeviceMemory;

	std::vector<uint16_t> m_Indexes;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

protected:
	std::vector<Vertex>m_Vertices;
};