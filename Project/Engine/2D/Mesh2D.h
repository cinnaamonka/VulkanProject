#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "../Structs.h"
#include "../CommandPool.h"
#include "../IndexBuffer.h"
#include "../DaeDataBuffer.h"

class CommandPool;

class Mesh2D
{
public:
	Mesh2D();  
	virtual ~Mesh2D() {};

	Mesh2D::Mesh2D(const Mesh2D& other)
		: m_Vertices(other.m_Vertices),
		m_Indexes(other.m_Indexes),
		m_MaxFramesInFlight(other.m_MaxFramesInFlight),
		m_VertexConstant(other.m_VertexConstant)
	{
		m_VertexBuffer = std::make_unique<DAEDataBuffer>(*other.m_VertexBuffer);
		m_IndexBuffer = std::make_unique<DAEDataBuffer>(*other.m_IndexBuffer);

		m_UniformBuffers = other.m_UniformBuffers;
		m_UniformBuffersMemory = other.m_UniformBuffersMemory;
		m_UniformBuffersMapped = other.m_UniformBuffersMapped;
	}

	void DestroyMesh(const VkDevice& device);

	void AddVertex(glm::vec2 pos, glm::vec3 color);

	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const std::vector<Vertex> vertexes,
		const VkQueue& graphicsQueue,const CommandPool& commandPool, std::vector<uint16_t> indices);

	void Draw(const VkPipelineLayout& pipelineLayout, const VkCommandBuffer& commandBuffer);

	void CreateUniformBuffers(const VkDevice& device, const VkPhysicalDevice& physcialDevice);
	void DestroyUniformBuffers(const VkDevice& device);
	 
private:

	std::vector<uint16_t> m_Indexes;

	std::unique_ptr<DAEDataBuffer> m_VertexBuffer;
	std::unique_ptr<DAEDataBuffer> m_IndexBuffer;

	std::vector<VkBuffer> m_UniformBuffers;
	std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	std::vector<void*> m_UniformBuffersMapped;

	const uint32_t m_MaxFramesInFlight;

	VertexConstant m_VertexConstant;

protected:
	std::vector<Vertex>m_Vertices;
};