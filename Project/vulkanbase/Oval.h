#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"

class Oval: public Mesh
{
public:
	Oval() {};
	~Oval() {};

	void InitializeOval(const VkPhysicalDevice& physicalDevice, const VkDevice& device, glm::vec2 center,float radius,int numVertices);

private:
	 float m_Radius = 0.5f;
	 
	 glm::vec2 m_Center;

	 int m_VertixesNum;
};