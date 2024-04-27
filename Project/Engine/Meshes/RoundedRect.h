#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "../2D/Mesh2D.h"  

class CommandPool;

class RoundedRect : public Mesh2D
{
public: 
	RoundedRect(glm::vec2 leftPos, double width, double height, double radius, int numOfSegments);
	~RoundedRect() {};

	void InitializeRoundedRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const VkQueue& graphicsQueue,
		const CommandPool& commandPool, ImageManager& imageManager);

	void GenerateRoundedRectVertices(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, const float radians);
	void AddRectangles(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, const std::vector<glm::vec2>& cornerVertices);
	void AddRect(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomRight, const glm::vec2& bottomLeft);

private:

	glm::vec2 m_LeftPos;
	double m_Width;
	double m_Height;
	double m_Radius;
	int m_SegmentsNum;

};
