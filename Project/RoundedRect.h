#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"  

class RoundedRect : public Mesh
{
public: 
	RoundedRect() {};
	~RoundedRect() {};

	void InitializeRoundedRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, glm::vec2 center,double width,
		double height, float radius, int numVertices);
private:
	glm::vec2 m_LeftPos;
	double m_Width;
	double m_Height;
	double m_Radius;
	int m_SegmentsNum;

};
