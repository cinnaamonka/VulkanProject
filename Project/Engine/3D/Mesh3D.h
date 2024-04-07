#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <vulkan/vulkan.h>
#include "../DaeDataBuffer.h"
#include "../Structs.h"
#include <algorithm>

class Mesh3D
{
public:
	Mesh3D();
	~Mesh3D() {};

    Mesh3D::Mesh3D(const Mesh3D& other)
        : m_Vertices(other.m_Vertices),
        m_Indices(other.m_Indices),
		m_ModelIndices(other.m_ModelIndices),
        m_MaxFramesInFlight(other.m_MaxFramesInFlight),
        m_VertexConstant(other.m_VertexConstant)
    {
		m_VertexBuffer = std::make_unique<DAEDataBuffer>(*other.m_VertexBuffer); 
		m_IndexBuffer = std::make_unique<DAEDataBuffer>(*other.m_IndexBuffer);
		m_UniformBuffers = other.m_UniformBuffers;
		m_UniformBuffersMemory = other.m_UniformBuffersMemory;
		m_UniformBuffersMapped = other.m_UniformBuffersMapped;
    }

	void Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device,
		const VkQueue& graphicsQueue, const CommandPool& commandPool);
	void InitializeModel(const VkPhysicalDevice& physicalDevice, const VkDevice& device,
		const VkQueue& graphicsQueue, const CommandPool& commandPool);
	void DestroyMesh(const VkDevice& device);
	void AddVertex(glm::vec3 pos, glm::vec3 color);
	void AddTriangle(uint16_t i1, uint16_t i2, uint16_t i3, uint16_t offset = 0);

	void Draw(const VkPipelineLayout& pipelineLayout,const VkCommandBuffer& commandBuffer);

	void CreateUniformBuffers(const VkDevice& device, const VkPhysicalDevice& physcialDevice);
	void DestroyUniformBuffers(const VkDevice& device);

	std::vector<Vertex3D> GetVertices() const 
	{
		return m_Vertices;
	}

	void SetVertices(std::vector<Vertex3D> vertices)
	{
		m_Vertices = vertices;
	}
	void SetModelIndexes(std::vector<uint32_t > indexes)
	{
		m_ModelIndices = indexes;
	}

	std::vector<uint32_t> GetModelIndices() const 
	{
		return m_ModelIndices;
	}

private:
	std::vector<Vertex3D> m_Vertices;
	std::vector<uint16_t > m_Indices;
	std::vector<uint32_t > m_ModelIndices;
	std::unique_ptr<DAEDataBuffer > m_VertexBuffer;
	std::unique_ptr<DAEDataBuffer > m_IndexBuffer;
	
	std::vector<VkBuffer> m_UniformBuffers;
	std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	std::vector<void*> m_UniformBuffersMapped;

	const uint32_t m_MaxFramesInFlight;

	VertexConstant m_VertexConstant;
};