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
		const VkQueue& graphicsQueue, const CommandPool& commandPool, ImageManager& imageManager);
	void InitializeModel(const VkPhysicalDevice& physicalDevice, const VkDevice& device,
		const VkQueue& graphicsQueue, const CommandPool& commandPool, ImageManager& imageManager);
	void DestroyMesh(const VkDevice& device);
	void AddVertex(const glm::vec3& pos,const glm::vec3& color,const glm::vec3& normal);
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
	std::vector<uint16_t> GetIndices() const
	{
		return m_Indices;
	}
	void InitializeCube(const glm::vec3& bottomLeftBackCorner, float sideLength)
	{
		m_Vertices.clear();
		m_Indices.clear();
		const glm::vec3 forward{ glm::vec3(0.0f, 0.0f, 1.0f) };
		const glm::vec3 up{ glm::vec3(0.0f, 1.0f, 0.0f) };
		const glm::vec3 right{ glm::vec3(1.0f, 0.0f, 0.0f) };

		const glm::vec3 bottomRightBackCorner = bottomLeftBackCorner + sideLength * right;
		const glm::vec3 topLeftBackCorner = bottomLeftBackCorner + sideLength * up;
		const glm::vec3 topRightBackCorner = bottomRightBackCorner + sideLength * up;
		const glm::vec3 bottomLeftFrontCorner = bottomLeftBackCorner + sideLength * forward;
		const glm::vec3 bottomRightFrontCorner = bottomRightBackCorner + sideLength * forward;
		const glm::vec3 topLeftFrontCorner = topLeftBackCorner + sideLength * forward;
		const glm::vec3 topRightFrontCorner = topRightBackCorner + sideLength * forward;

		Vertex3D bottomLeftBackCornerVertex{ bottomLeftBackCorner };
		Vertex3D topRightFrontCornerVertex{ topRightFrontCorner };
		Vertex3D bottomRightBackCornerVertex{ bottomRightBackCorner };
		Vertex3D topLeftFrontCornerVertex{ topLeftFrontCorner };
		Vertex3D bottomLeftFrontCornerVertex{ bottomLeftFrontCorner };
		Vertex3D topRightBackCornerVertex{ topRightBackCorner };
		Vertex3D bottomRightFrontCornerVertex{ bottomRightFrontCorner };
		Vertex3D topLeftBackCornerVertex{ topLeftBackCorner };

		AddRectPlane(bottomLeftBackCornerVertex, topLeftBackCornerVertex,
			topRightBackCornerVertex, bottomRightBackCornerVertex, true, false); // back plane
		AddRectPlane(bottomLeftFrontCornerVertex, topLeftFrontCornerVertex,
			topRightFrontCornerVertex, bottomRightFrontCornerVertex, false, false); // front plane
		AddRectPlane(bottomLeftBackCornerVertex, topLeftBackCornerVertex,
			topLeftFrontCornerVertex, bottomLeftFrontCornerVertex, false, false); // left plane
		AddRectPlane(bottomRightBackCornerVertex, topRightBackCornerVertex,
			topRightFrontCornerVertex, bottomRightFrontCornerVertex, true, false); // right plane
		AddRectPlane(topLeftBackCornerVertex, topLeftFrontCornerVertex, topRightFrontCornerVertex, topRightBackCornerVertex,
			true, false); // top plane
		AddRectPlane(bottomLeftBackCornerVertex, bottomLeftFrontCornerVertex,
			bottomRightFrontCornerVertex, bottomRightBackCornerVertex, false, false); // bottom plane
	}

	void AddRectPlane(Vertex3D& bottomLeft, Vertex3D& topLeft, Vertex3D& topRight, Vertex3D& bottomRight,
		bool isClockWise, bool keepNormals = true) {

		if (!keepNormals)
		{
			glm::vec3 normal = glm::normalize(glm::cross(topLeft.position - bottomLeft.position, bottomRight.position - bottomLeft.position));
			if (!isClockWise) normal *= -1;
			bottomLeft.normal = normal;
			topLeft.normal = normal;
			bottomRight.normal = normal;
			topRight.normal = normal;
		}
		bottomLeft.texCoord = glm::vec2(1.0f, 0.0f); 
		topLeft.texCoord = glm::vec2(0.0f, 0.0f); 
		topRight.texCoord = glm::vec2(0.0f, 1.0f); 
		bottomLeft.texCoord = glm::vec2(1.0f, 1.0f); 
		bottomRight.texCoord = glm::vec2(1.0f, 0.0f);

		if (isClockWise) {
			
			AddVertex(bottomLeft);
			AddVertex(topLeft);
			AddVertex(topRight);
			AddVertex(bottomLeft);
			AddVertex(topRight);
			AddVertex(bottomRight);
		}
		else {
			AddVertex(bottomLeft);
			AddVertex(topRight);
			AddVertex(topLeft);
			AddVertex(bottomLeft);
			AddVertex(bottomRight);
			AddVertex(topRight);
		}

	}
	void Mesh3D::AddVertex(const Vertex3D& vertex)
	{
		m_Vertices.push_back(vertex);
		m_Indices.push_back(static_cast<uint16_t>(m_Indices.size()));
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