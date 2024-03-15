#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Structs.h"
#include "CommandPool.h"

class CommandPool;

class Mesh
{
public:
	Mesh() {};
	virtual ~Mesh() {};

	void DestroyMesh(const VkDevice& device);

	void Draw(const VkCommandBuffer& buffer) const;

	void AddVertex(glm::vec2 pos, glm::vec3 color);

	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const std::vector<Vertex> vertexes, const VkQueue& graphicsQueue,const CommandPool& commandPool);
	 
private:
	
	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkDeviceMemory;

protected:
	std::vector<Vertex>m_Vertices;
};