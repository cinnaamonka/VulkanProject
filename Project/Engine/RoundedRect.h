#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"  

class CommandPool;

class RoundedRect : public Mesh
{
public: 
	RoundedRect(glm::vec2 leftPos, double width, double height, double radius, int numOfSegments);
	~RoundedRect() {};

	void InitializeRoundedRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const VkQueue& graphicsQueue,
		const CommandPool& commandPool);
private:
	glm::vec2 m_LeftPos;
	double m_Width;
	double m_Height;
	double m_Radius;
	int m_SegmentsNum;

};
