#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "../Structs.h"
#include "../CommandPool.h"
#include "../IndexBuffer.h"

class CommandPool;

class Mesh2D
{
public:
	Mesh2D();  
	virtual ~Mesh2D() {};

	void DestroyMesh(const VkDevice& device, const VkDescriptorSetLayout& layout);

	void Draw(const VkCommandBuffer& buffer) const;

	void AddVertex(glm::vec2 pos, glm::vec3 color);

	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const std::vector<Vertex> vertexes,
		const VkQueue& graphicsQueue,const CommandPool& commandPool, std::vector<uint16_t> indices);

	void CreateUniformBuffers(const VkDevice& device, const VkPhysicalDevice& physcialDevice);
	void DestroyUniformBuffers(const VkDevice& device,const VkDescriptorSetLayout& descriptiveSetLayout);

	void UpdateUniformBuffer(uint32_t currentImage, const VkExtent2D& swapChainExtent);
	 
private:
	
	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkDeviceMemory;

	std::vector<uint16_t> m_Indexes;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> m_UniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	const uint32_t m_MaxFramesInFlight;

protected:
	std::vector<Vertex>m_Vertices;
};